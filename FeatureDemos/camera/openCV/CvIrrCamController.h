#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <irrlicht.h>
#include <math.h>

//defines
#define MAX_TRACKING_POINTS 10

//I'm lazy
using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class CvIrrCamController
{
	public:
		CvIrrCamController(char *cascade_filename = 0);
		~CvIrrCamController();

		int loadCascade(char *filename);
		int startWebcam(int cvflags = 0);

		void setCamera(ICameraSceneNode* camera);
		void setCameraCenter(vector3df cam_center);
		void setCameraViewWindowCenter(vector3df cam_window_center);
		void setCalib(float new_cam_mat[3][3], float new_lens_dist[4]);
		void setCalibExtrinsic(vector3df translation, vector3df rotation);
		void setMaxTravel(vector3df travel_dimensions);
		void setScaleTravel(vector3df travel_scale);
		vector3df getMaxTravel();
		void setViewWindowSize(vector2df window_dimensions);
		vector2df getViewWindowSizel();

		int UpdateCamPosition();

		void ShowDebugWindow();
		void HideDebugWindow();

	private:
		int find_face(IplImage* grey_img, vector3df *offset, CvRect *found_face_box);
		int find_points(IplImage* grey_img, CvRect work_area);
		void track_points();
		int follow_points(IplImage* grey_img, vector3df *offset, CvPoint2D32f *new_set, int max_points);
		int show_debug_img(IplImage* image, CvRect* face_area);
		vector3df make_new_cam_position(vector3df offset);
		void init_tracking_vars(IplImage* reference_img);
		void relocate_cam(vector3df new_position);
		CvPoint2D32f average_point(CvPoint2D32f *point_array, int max_points);
		CvPoint2D32f standard_deviation_point(CvPoint2D32f *point_array, int max_points);
		int abandon_point(CvPoint2D32f point);
		float vector_magnitude(vector3df vector);
		float* create_glfrustumLH(float left, float right, float bottom, float top, float nearZ, float farZ, float *m);
		float* create_glfrustumRH(float left, float right, float bottom, float top, float nearZ, float farZ, float *m);
		void make_window_effect(vector3df offset);
		void calibrate_offset(vector3df *offset);
		void check_offset_maximums(vector3df *offset);
		void smooth_offset_change(vector3df *offset);
		void correct_lens_distortion(IplImage* dst);
		float determine_depth(vector3df *offset);
		vector3df return_ray(vector3df pixel_cord);

		//used by opencv for face detection
		CvMemStorage* storage;
		CvHaarClassifierCascade* cascade;
		CvCapture* cv_capture;

		//more stuff used for face detection and tracking
		IplImage* grey_img;
		IplImage* prev_grey_img;
		IplImage* pyramid_img;
		IplImage* prev_pyramid_img;
		CvPoint2D32f *tracking_points;
		CvPoint2D32f *prev_tracking_points;
		CvPoint2D32f last_tracking_points_ave;
		CvPoint2D32f last_tracking_points_std;
		int points_found;
		char point_status[MAX_TRACKING_POINTS];
		vector3df old_offset, last_tracking_offset;

		//our Camera
		ICameraSceneNode* camera;

		//max values in "2d" space our camera can move to from the center point
		vector3df scale_travel;
		vector3df max_travel;

		//frustum values
		float view_width;
		float view_height;

		//calib values
		float cam_mat[3][3];
		float lens_dist[4];
		CvMat *cv_cam_mat;
		CvMat *cv_inv_cam_mat;
		CvMat *cv_lens_dist;
		vector3df calib_origin;
		vector3df calib_far;
		matrix4 calib_extrinsic;

		//Camera's Center Reference Point
		bool cam_center_set;
		bool cam_window_set;
		vector3df cam_center;
		vector3df cam_window_center;

		//and this...
		int show_debug_window;
		char cv_window_name[100];
};
