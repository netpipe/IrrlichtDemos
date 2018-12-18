#include "CvIrrCamController.h"

CvIrrCamController::CvIrrCamController(char *cascade_filename)
{
	//make unique cv_window name incase we turn debuging on
	sprintf(this->cv_window_name, "CvIrrCamController%d", this);
	
	this->cascade = 0;
	this->storage = 0;
	
	this->grey_img = 0;
	this->prev_grey_img = 0;
	this->pyramid_img = 0;
	this->prev_pyramid_img = 0;
	this->points_found = 0;
	
	this->cam_center_set = false;
	this->cam_window_set = false;
	
	//set some default values
	this->setMaxTravel(vector3df(100,100,100));
	this->setViewWindowSize(vector2df(640 / 4,480 / 4));
	this->setScaleTravel(vector3df(1,1,1));
	
	//default calibration values
	this->cv_cam_mat = cvCreateMat(3,3,CV_32FC1);
	this->cv_inv_cam_mat = cvCreateMat(3,3,CV_32FC1);
    this->cv_lens_dist = cvCreateMat(4,1,CV_32FC1);
    
	float cam_mat[3][3];

	cam_mat[0][0] = 1.0;
	cam_mat[0][1] = 0.0;
	cam_mat[0][2] = 0.0;

	cam_mat[1][0] = 0.0;
	cam_mat[1][1] = 1.0;
	cam_mat[1][2] = 0.0;

	cam_mat[2][0] = 0.0;
	cam_mat[2][1] = 0.0;
	cam_mat[2][2] = 1.0;

	float lens_dist[4];

	lens_dist[0] = -0.01109;
	lens_dist[1] = -0.02224;
	lens_dist[2] = -0.00830;
	lens_dist[3] = -0.00075;

	this->setCalib(cam_mat, lens_dist);
	
	//default
	this->setCalibExtrinsic(vector3df(0,0,0), vector3df(0,0,0));
	
	//alloc
	this->tracking_points = (CvPoint2D32f*)cvAlloc(MAX_TRACKING_POINTS*sizeof(this->tracking_points[0]));
	this->prev_tracking_points = (CvPoint2D32f*)cvAlloc(MAX_TRACKING_POINTS*sizeof(this->tracking_points[0]));
	
	//create storage needed for head tracking
	this->storage = cvCreateMemStorage(0);
	
	//load file if given
	if(cascade_filename)
		this->loadCascade(cascade_filename);
	
	//have this default off
	this->show_debug_window = 0;
}

CvIrrCamController::~CvIrrCamController()
{
	//free up what we took
	if(this->cascade) 	cvReleaseHaarClassifierCascade(&this->cascade);
	if(this->storage) 	cvReleaseMemStorage(&this->storage);
	if(this->grey_img) 	cvReleaseImage(&this->grey_img);
	if(this->prev_grey_img) cvReleaseImage(&this->prev_grey_img);
	if(this->pyramid_img) 	cvReleaseImage(&this->pyramid_img);
	if(this->prev_pyramid_img) cvReleaseImage(&this->prev_pyramid_img);
}

int CvIrrCamController::loadCascade(char *filename)
{
	//cascade is a trained set of data for face matching
	this->cascade = (CvHaarClassifierCascade*)cvLoad( filename, 0, 0, 0 );
	
	//let the people know
	if(this->cascade)
		return 1;
	else
		return 0;
}

void CvIrrCamController::setCamera(ICameraSceneNode* camera)
{
	this->camera = camera;

	//if they weren't manually set, then this is a good estimator
	if (!this->cam_center_set) this->setCameraCenter(camera->getPosition());
	if (!this->cam_window_set) this->setCameraViewWindowCenter(camera->getTarget());
}

void CvIrrCamController::setCameraCenter(vector3df cam_center)
{
	this->cam_center = cam_center;
	
	//yes
	this->cam_center_set = true;
}

void CvIrrCamController::setCameraViewWindowCenter(vector3df cam_window_center)
{
	this->cam_window_center = cam_window_center;
	
	//yes
	this->cam_window_set = true;
}

int CvIrrCamController::startWebcam(int cvflags)
{
	//cvflags defaults to 0
	this->cv_capture = cvCaptureFromCAM(cvflags);
	
	if(this->cv_capture)
		return 1;
	else
		return 0;
}

int CvIrrCamController::UpdateCamPosition()
{
	IplImage* cap_image = 0;
	IplImage* img = 0;
	vector3df offset, new_cam_pos;
	CvRect face_loc;
	static int consecutive_faces = 0;
	
	//grab image, exit if fail
	if(!(cap_image = cvQueryFrame(this->cv_capture))) return 0;
	
	//is the origin of the x,y in the Top Left?
	if( cap_image->origin != IPL_ORIGIN_TL ) 
	{
		if( !img ) img = cvCreateImage( cvGetSize(cap_image), cap_image->depth, cap_image->nChannels );

		cvFlip( cap_image, img, 0 );
		cap_image = img;
	}
	
	//undistort
	this->correct_lens_distortion(cap_image);
	
	//init stuff?
	if(!this->grey_img) this->init_tracking_vars(cap_image);
	
	//make the grey version
	cvCvtColor( cap_image, this->grey_img, CV_BGR2GRAY );
	
	//find the face in the image
	int face_found = this->find_face(this->grey_img, &offset, &face_loc);
	
	//needed to stop flickering while in point tracking mode
	if(face_found) consecutive_faces++;
	else consecutive_faces = 0;
	
	const int min_consecutive_faces = 3;
	if(!face_found || (face_found && consecutive_faces < min_consecutive_faces && this->points_found)) 
	{
		//track by points instead
		if(this->points_found)
		{
			//opencv tracking algorithm
			this->track_points();
			
			//our offset estimator
			if(this->follow_points(this->grey_img, &offset, this->tracking_points, this->points_found))
			{
				//regulate change speed
				this->smooth_offset_change(&offset);
				
				//make and relocate
				new_cam_pos = this->make_new_cam_position(offset);
				
				this->relocate_cam(new_cam_pos);
				
				//display
				this->show_debug_img(cap_image, NULL);
		
				return 1;
			}
		}
		
		//display
		if(face_found) this->show_debug_img(cap_image, &face_loc);
		else this->show_debug_img(cap_image, NULL);
		
		return 0;
	}
	
	
	//find features points used for tracking head better
	points_found = this->find_points(this->grey_img, face_loc);
	
	//show debug window
	this->show_debug_img(cap_image, &face_loc);

	//regulate change speed
	this->smooth_offset_change(&offset);
	
	//find the new camera position based off our 3d head cordinates
	new_cam_pos = this->make_new_cam_position(offset);
	
	//Move the cam accordingly
	this->relocate_cam(new_cam_pos);
	
	return 1;
}

vector3df CvIrrCamController::make_new_cam_position(vector3df offset)
{
	float theta1, theta2, theta3, theta4;
	
	//calibrate the ol' offset
	this->calibrate_offset(&offset);
	printf("\nposition in inches: %f %f %f\n",offset.X, offset.Y, offset.Z);
	//scale it
	offset *= this->scale_travel;
	
	//double check maximums
	this->check_offset_maximums(&offset);
	
	//do this..
	offset.Z = -offset.Z;
	//offset.Y = -offset.Y;
	
	//Do some math to the cam's new position
	float cam_x_offset = this->cam_center.X - this->cam_window_center.X;
	float cam_y_offset = this->cam_center.Y - this->cam_window_center.Y;
	float cam_z_offset = this->cam_center.Z - this->cam_window_center.Z;
	float cam_mag = sqrt(cam_x_offset * cam_x_offset + cam_y_offset * cam_y_offset + cam_z_offset * cam_z_offset);
	theta1 = atan2(cam_x_offset, cam_z_offset);
	theta2 = atan2(cam_y_offset, cam_x_offset);
	theta3 = atan2(cam_y_offset, cam_z_offset);
	theta4 = asin(cam_y_offset / cam_mag);

	theta1 += (3.14 / 2);
	theta2 += (3.14 / 2);
	theta3 += (3.14 / 2);
	
	//create
	vector3df new_cam_pos;
	//transform X + Z in relation to x + z offsets
	new_cam_pos.X = sin(theta1) * offset.X;
	new_cam_pos.Z = cos(theta1) * offset.X;
	//transform X + Y + Z in relation to Y
	new_cam_pos.Y = cos(theta4) * offset.Y;
	new_cam_pos.X += cos(theta2) * offset.Y;
	new_cam_pos.Z += cos(theta3) * offset.Y;
	
	//Depth shift by unit vector from cam to origin
	new_cam_pos.X -= offset.Z * cam_x_offset / cam_mag;
	new_cam_pos.Y -= offset.Z * cam_y_offset / cam_mag;
	new_cam_pos.Z -= offset.Z * cam_z_offset / cam_mag;
	
	//shift
	new_cam_pos += this->cam_center;

	//projection matrix
	offset.Z = -offset.Z;
	this->make_window_effect(offset);
	
	return new_cam_pos;
}

void CvIrrCamController::make_window_effect(vector3df offset)
{
	matrix4 projection_matrix;
	float m[16];
	float xleft, xright, ybottom, ytop, znear, zfar;
	float magnitude;
	
	//how far away is the cam from the viewing plane?
	magnitude = this->vector_magnitude(this->cam_center - this->cam_window_center);

	//initials
	xleft = -this->view_width / 2;
	ybottom = -this->view_height / 2;
	znear = magnitude;
	
	//setters
	xleft -= offset.X;
	ybottom -= offset.Y;
	znear += offset.Z;
	
	//final shifts
	xright = xleft + this->view_width;
	ytop = ybottom + this->view_height;
	zfar = znear + 1000;

	this->create_glfrustumLH(xleft, xright, ybottom, ytop, znear, zfar, m);
		
	projection_matrix.setM(m);
		
	this->camera->setProjectionMatrix(projection_matrix);
}

void CvIrrCamController::smooth_offset_change(vector3df *offset)
{
	float move_speed;
	float min_speed = 0.5;
	vector3df direction_vector;
	static vector3df current_position = *offset;

	//get the directional unit vector
	direction_vector = *offset - current_position;
	float magnitude = this->vector_magnitude(direction_vector);
	direction_vector /= magnitude;
	
	//set move speed
	move_speed = magnitude * 0.4;
	
	//check if it is too slow
	if(move_speed < min_speed) move_speed = min_speed;
	
	//change new_position if ok
	if(magnitude > move_speed)
	{
		direction_vector *= move_speed;
		*offset = current_position + direction_vector;
	}

	//for next round
	current_position = *offset;
}

void CvIrrCamController::relocate_cam(vector3df new_position)
{
	//set cam position
	this->camera->setPosition(new_position);
	
	//now relocate the target too
	vector3df offset = new_position - this->cam_center;
	this->camera->setTarget(offset + this->cam_window_center);
}

int CvIrrCamController::find_face(IplImage* process_image, vector3df *offset, CvRect *found_face_box)
{
	static CvScalar color = {0,0,255};
	static IplImage* small_img;
	double scale = 1.3;
	
	//make a smaller version to speed up detection processs
	if(!small_img) small_img = cvCreateImage( cvSize( cvRound (process_image->width/scale), cvRound (process_image->height/scale)), 8, 1 );

	//these are used to speed up the detection process
	cvResize( process_image, small_img, CV_INTER_LINEAR );
	cvEqualizeHist( small_img, small_img );
	cvClearMemStorage( this->storage );
	
	//detect faces
	CvSeq* faces = cvHaarDetectObjects( small_img, this->cascade, this->storage, 1.1, 2, 0, cvSize(30, 30) );
	
	//if no faces then exit
	if(!faces->total)
		return 0;
	
	//get rectangle over the face
	CvRect* r = (CvRect*)cvGetSeqElem( faces, 0 );
	
	//rescale it
	r->x = (int)(r->x * scale);
	r->y = (int)(r->y * scale);
	r->width = (int)(r->width * scale);
	r->height = (int)(r->height * scale);
	
	//set this for reference outside this function
	*found_face_box = *r;
	
	offset->X = r->x + r->width * 0.5;
	offset->Y = r->y + r->height * 0.5;
	offset->Z = 1.0 * r->height / process_image->height;

	//needed in point tracking
	this->old_offset = *offset;
	this->last_tracking_offset = *offset;

	return 1;
}

int CvIrrCamController::follow_points(IplImage* process_image, vector3df *offset, CvPoint2D32f *new_set, int max_points)
{
	CvPoint2D32f new_ave, new_std;
	float new_std_mag, old_std_mag, depth_percent_change;
	const float max_change = 40;
	vector3df offset_tobe;
	
	if(!max_points) return 0;
	
	//ave determines x,y and std determines depth
	new_ave = this->average_point(new_set, max_points);
	new_std = this->standard_deviation_point(new_set, max_points);
	
	//magnitudes
	new_std_mag = sqrt(new_std.x * new_std.x + new_std.y * new_std.y);
	old_std_mag = sqrt(this->last_tracking_points_std.x * this->last_tracking_points_std.x + this->last_tracking_points_std.y * this->last_tracking_points_std.y);
	
	//percentage changed
	depth_percent_change = new_std_mag / old_std_mag;
	
	//set some stuff
	offset_tobe.X = this->last_tracking_points_ave.x - new_ave.x;
	offset_tobe.Y = this->last_tracking_points_ave.y - new_ave.y;
	offset_tobe.Z = (depth_percent_change - 1);

	//shift it
	offset_tobe += this->old_offset;
	
	//okay is it bad?
	if(max_change < this->vector_magnitude(offset_tobe - this->last_tracking_offset)) return 0;
	
	*offset = offset_tobe;
	
	//set this
	this->last_tracking_offset = *offset;
	
	return 1;
}

int CvIrrCamController::find_points(IplImage* image, CvRect work_area)
{
	IplImage *result;
	IplImage* eig_image;
	IplImage* temp_image;
	double scale = 0.5; //make sure we are finding points ONLY on the face
	
	//scale it
	work_area.x += (work_area.width - (int)(work_area.width * scale)) / 2;
	work_area.y += (work_area.height - (int)(work_area.height * scale)) / 2;
	work_area.width = (int)(work_area.width * scale);
	work_area.height = (int)(work_area.height * scale);
	
	//focus on the head area
	cvSetImageROI( image, work_area);

	//make the sub-image
	result = cvCreateImage( cvSize(work_area.width, work_area.height), image->depth, image->nChannels );
	cvCopy(image,result);
	
	//go back to hell
	cvResetImageROI(image);

	//create these lovelies
	double quality = 0.01;
	double min_distance = 10;
	this->points_found = MAX_TRACKING_POINTS;
	
	//must be made with the rest of them
	eig_image = cvCreateImage( cvGetSize(result), 32, 1 );
	temp_image = cvCreateImage( cvGetSize(result), 32, 1 );
	
	//actually find the points
	cvGoodFeaturesToTrack( result, eig_image, temp_image, this->tracking_points, &this->points_found, quality, min_distance, 0, 3, 0, 0.04 );

	//shift the points
	for(int i=0;i<this->points_found;i++)
	{
		CvPoint pt = cvPointFrom32f(this->tracking_points[i]);
		this->tracking_points[i] = cvPoint2D32f(pt.x + work_area.x,pt.y + work_area.y);
		this->prev_tracking_points[i] = cvPoint2D32f(pt.x + work_area.x,pt.y + work_area.y);
	}
	
	//release
	cvReleaseImage(&result);
	cvReleaseImage(&eig_image);
	cvReleaseImage(&temp_image);
	
	//now this needs done too
	cvCopy(this->grey_img, this->prev_grey_img);
	
	//need this set for point tracking
	this->last_tracking_points_ave = this->average_point(this->tracking_points, this->points_found);
	this->last_tracking_points_std = this->standard_deviation_point(this->tracking_points, this->points_found);

	return this->points_found;
}

void CvIrrCamController::track_points()
{
	static int flags = 0;
	int i,k;
	CvPoint2D32f *swap_points;
	IplImage *swap_temp;
	
	cvCalcOpticalFlowPyrLK( this->prev_grey_img, this->grey_img, this->prev_pyramid_img, this->pyramid_img, this->prev_tracking_points, this->tracking_points, this->points_found, cvSize(10, 10), 3, this->point_status, 0, cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03), flags );
	
	flags |= CV_LKFLOW_PYR_A_READY;
	
	//now how many of these do we have left?
	for(i=0,k=0;i < this->points_found;i++)
		if(this->point_status[i] && !this->abandon_point(this->tracking_points[i]))
			this->tracking_points[k++] = this->tracking_points[i];
	
	this->points_found = k;
	
	//swap stuff
	CV_SWAP( this->prev_grey_img, this->grey_img, swap_temp );
	CV_SWAP( this->prev_pyramid_img, this->pyramid_img, swap_temp );
	CV_SWAP( this->prev_tracking_points, this->tracking_points, swap_points );
}

int CvIrrCamController::show_debug_img(IplImage* image, CvRect* face_area)
{
	int i;
	
	//do it at all?
	if(!this->show_debug_window) return 0;

	//draw the rectangle if we got one
	if(face_area)
	{
		CvPoint center;
		int radius;
		
		cvRectangle(image, cvPoint(face_area->x, face_area->y), cvPoint(face_area->x + face_area->width,face_area->y + face_area->height), cvScalar(0,0,255), 1);
	}
	
	//draw the points if we got them
	for(i = 0;i < this->points_found;i++)
		cvCircle( image, cvPointFrom32f(this->tracking_points[i]), 3, CV_RGB(200,0,0), 0, 8,0);
	
	//show some feature tracking stuff
	if(this->points_found)
	{
		CvPoint2D32f the_average, the_std;
		
		the_average = this->average_point(this->tracking_points, this->points_found);
		the_std = this->standard_deviation_point(this->tracking_points, this->points_found);
		
		cvCircle( image, cvPointFrom32f(the_average), 5, CV_RGB(0,0,255), 0, 8,0);
		
	}
	
	cvShowImage(this->cv_window_name, image);
	cvWaitKey(10);
}

void CvIrrCamController::init_tracking_vars(IplImage* reference_img)
{
	if(!this->grey_img) 		this->grey_img 		= cvCreateImage( cvGetSize(reference_img), 8, 1 );
	if(!this->prev_grey_img) 	this->prev_grey_img 	= cvCreateImage( cvGetSize(reference_img), 8, 1 );
	if(!this->pyramid_img) 		this->pyramid_img 	= cvCreateImage( cvGetSize(reference_img), 8, 1 );
	if(!this->prev_pyramid_img) 	this->prev_pyramid_img 	= cvCreateImage( cvGetSize(reference_img), 8, 1 );
}

void CvIrrCamController::ShowDebugWindow()
{
	//create window
	cvNamedWindow(this->cv_window_name,1); 
	
	//set this
	this->show_debug_window = 1;
}

void CvIrrCamController::HideDebugWindow()
{
	this->show_debug_window = 0;
	
	//kill kill kill, the white window
	cvDestroyWindow(this->cv_window_name); 
}

CvPoint2D32f CvIrrCamController::average_point(CvPoint2D32f *point_array, int max_points)
{
	CvPoint2D32f the_average;
	
	the_average.x = 0;
	the_average.y = 0;
	
	if(!max_points) return the_average;
	
	for(int i=0;i<max_points;i++)
	{
		the_average.x += point_array[i].x;
		the_average.y += point_array[i].y;
	}
	
	the_average.x /= max_points;
	the_average.y /= max_points;

	
	return the_average;
}

CvPoint2D32f CvIrrCamController::standard_deviation_point(CvPoint2D32f *point_array, int max_points)
{
	CvPoint2D32f the_average_squared;
	CvPoint2D32f the_std;
	
	the_std.x = 0;
	the_std.y = 0;
	
	if(!max_points) return the_std;
	
	the_average_squared = this->average_point(point_array, max_points);
	
	the_average_squared.x *= the_average_squared.x;
	the_average_squared.y *= the_average_squared.y;
	
	for(int i=0;i<max_points;i++)
	{
		the_std.x += point_array[i].x * point_array[i].x;
		the_std.y += point_array[i].y * point_array[i].y;
	}
	
	the_std.x = sqrt((the_std.x / max_points) - the_average_squared.x);
	the_std.y = sqrt((the_std.y / max_points) - the_average_squared.y);
	
	return the_std;
}

int CvIrrCamController::abandon_point(CvPoint2D32f point)
{
	const float edge_percentage = 0.01;
	static int max_x = 0, min_x = 0, max_y = 0, min_y = 0;
	
	//this function is currently just a simple check if the point is too close to the boundary
	
	if(!max_x)
	{
		min_x = (int)(this->grey_img->width * edge_percentage);
		max_x = this->grey_img->width;
		min_y = (int)(this->grey_img->height * edge_percentage);
		max_y = this->grey_img->height;
	}
	
	if(point.x > max_x) return 1;
	if(point.x < min_x) return 1;
	if(point.y > max_y) return 1;
	if(point.y < min_y) return 1;
	
	return 0;
}

float CvIrrCamController::vector_magnitude(vector3df vector)
{
	return sqrt(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z);
}

float* CvIrrCamController::create_glfrustumLH(float left, float right, float bottom, float top, float nearZ, float farZ, float *m)
{
	float x = (2.0F*nearZ) / (right-left);
	float y = (2.0F*nearZ) / (top-bottom);
	float a = (right+left) / (right-left);
	float b = (top+bottom) / (top-bottom);
	float c = -(farZ+nearZ) / ( farZ-nearZ);
	float d = -(2.0F*farZ*nearZ) / (farZ-nearZ);

#define M(row,col)  m[col*4+row]
	M(0,0) = x;     M(0,1) = 0.0F;  M(0,2) = a;      M(0,3) = 0.0F;
	M(1,0) = 0.0F;  M(1,1) = y;     M(1,2) = b;      M(1,3) = 0.0F;
	M(2,0) = 0.0F;  M(2,1) = 0.0F;  M(2,2) = -c;      M(2,3) = d;
	M(3,0) = 0.0F;  M(3,1) = 0.0F;  M(3,2) = 1.0F;  M(3,3) = 0.0F;
#undef M
	
	return m;
}

float* CvIrrCamController::create_glfrustumRH(float left, float right, float bottom, float top, float nearZ, float farZ, float *m)
{
	float x = (2.0F*nearZ) / (right-left);
	float y = (2.0F*nearZ) / (top-bottom);
	float a = (right+left) / (right-left);
	float b = (top+bottom) / (top-bottom);
	float c = -(farZ+nearZ) / ( farZ-nearZ);
	float d = -(2.0F*farZ*nearZ) / (farZ-nearZ);

#define M(row,col)  m[col*4+row]
	M(0,0) = x;     M(0,1) = 0.0F;  M(0,2) = a;      M(0,3) = 0.0F;
	M(1,0) = 0.0F;  M(1,1) = y;     M(1,2) = b;      M(1,3) = 0.0F;
	M(2,0) = 0.0F;  M(2,1) = 0.0F;  M(2,2) = c;      M(2,3) = d;
	M(3,0) = 0.0F;  M(3,1) = 0.0F;  M(3,2) = -1.0F;  M(3,3) = 0.0F;
#undef M
	
	return m;
}

void CvIrrCamController::setCalib(float new_cam_mat[3][3], float new_lens_dist[4])
{
    int i, j, k;
    
    for(i=0;i<3;i++)
        for(j=0;j<3;j++)
        {
            cvmSet(this->cv_cam_mat,i,j,new_cam_mat[i][j]);
            this->cam_mat[i][j] = new_cam_mat[i][j];
        }
            
    for(k=0;k<4;k++)
    {
        cvmSet(this->cv_lens_dist,k,0,new_lens_dist[k]);            
        this->lens_dist[k] = new_lens_dist[k];
    }
    
    //make inversed
    cvInvert(this->cv_cam_mat,this->cv_inv_cam_mat);
}

void CvIrrCamController::calibrate_offset(vector3df *offset)
{
    float the_depth;
    
    the_depth = this->determine_depth(offset);
    
    *offset = this->return_ray(*offset);
    *offset *= the_depth;
    
    //now rotate
    this->calib_extrinsic.transformVect(*offset);
    this->calib_extrinsic.rotateVect(*offset);
}

vector3df CvIrrCamController::return_ray(vector3df pixel_cord)
{
    vector3df the_ray;
    
    the_ray.X = pixel_cord.X * cvmGet(this->cv_inv_cam_mat,0,0) + pixel_cord.Y * cvmGet(this->cv_inv_cam_mat,0,1) + cvmGet(this->cv_inv_cam_mat,0,2);
	the_ray.Y = pixel_cord.X * cvmGet(this->cv_inv_cam_mat,1,0) + pixel_cord.Y * cvmGet(this->cv_inv_cam_mat,1,1) + cvmGet(this->cv_inv_cam_mat,1,2);
	the_ray.Z = 1;
	
	return the_ray;
}

void CvIrrCamController::check_offset_maximums(vector3df *offset)
{
	if(offset->X > this->max_travel.X) offset->X = this->max_travel.X;
	if(offset->X < -this->max_travel.X) offset->X = -this->max_travel.X;
	if(offset->Y > this->max_travel.Y) offset->Y = this->max_travel.Y;
	if(offset->Y < -this->max_travel.Y) offset->Y = -this->max_travel.Y;
	if(offset->Z > this->max_travel.Z) offset->Z = this->max_travel.Z;
	if(offset->Z < -this->max_travel.Z) offset->Z = -this->max_travel.Z;
}

void CvIrrCamController::setMaxTravel(vector3df travel_dimensions)
{
	this->max_travel = travel_dimensions;
}

void CvIrrCamController::setScaleTravel(vector3df travel_scale)
{
     this->scale_travel = travel_scale;
}

vector3df CvIrrCamController::getMaxTravel()
{
	return this->max_travel;
}

void CvIrrCamController::setViewWindowSize(vector2df window_dimensions)
{
	this->view_width = window_dimensions.X;
	this->view_height = window_dimensions.Y;
}

vector2df CvIrrCamController::getViewWindowSizel()
{
	vector2df window_dimensions;
	
	window_dimensions.X = this->view_width;
	window_dimensions.Y = this->view_height;
	
	return window_dimensions;
}

void CvIrrCamController::correct_lens_distortion(IplImage* img)
{
     IplImage *temp_img = 0;
     
     if(!temp_img)
         temp_img = cvCreateImage( cvGetSize(img), img->depth, img->nChannels );
     
     cvCopy(img,temp_img);

     cvUndistort2(temp_img,img,this->cv_cam_mat,this->cv_lens_dist);
}

float CvIrrCamController::determine_depth(vector3df *offset)
{
    //need a head height for reference
    const float head_length = 7;
    float our_half_height = (offset->Z * this->grey_img->height) / 2.0;
    float our_depth;
    //make some copies
    vector3df bottom_ray, top_ray;
    
    //init
    bottom_ray = *offset - vector3df(0,our_half_height, 0);
    top_ray = *offset + vector3df(0,our_half_height, 0);
    
    bottom_ray = this->return_ray(bottom_ray);
    top_ray = this->return_ray(top_ray);
    
    //begin the calculation
    our_depth = top_ray.Y - bottom_ray.Y;
    our_depth = head_length / our_depth;

    return our_depth;
}


void CvIrrCamController::setCalibExtrinsic(vector3df translation, vector3df rotation)
{
    this->calib_extrinsic.setTranslation(translation);
    this->calib_extrinsic.setRotationDegrees(rotation);
}
