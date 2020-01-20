

float pi	= 3.14159265f;
float tol = 0.0001f;		// float type tolerance 



inline	float	DegreesToRadians(float deg)
{
	return deg * pi / 180.0f;
}

inline	float	RadiansToDegrees(float rad)
{	
	return rad * 180.0f / pi;
}














 	Quaternion operator+(Quaternion q1, Quaternion q2)

 
{
	return	Quaternion(	q1.n + q2.n,
							q1.v.X + q2.v.X,
							q1.v.Y + q2.v.Y,
							q1.v.Z + q2.v.Z);
}

	Quaternion operator-(Quaternion q1, Quaternion q2)
{
	return	Quaternion(	q1.n - q2.n,
							q1.v.X - q2.v.X,
							q1.v.Y - q2.v.Y,
							q1.v.Z - q2.v.Z);
}
 
	Quaternion operator*(Quaternion q1, Quaternion q2)
{
	return	Quaternion(	q1.n*q2.n - q1.v.X*q2.v.X - q1.v.Y*q2.v.Y - q1.v.Z*q2.v.Z,
							q1.n*q2.v.X + q1.v.X*q2.n + q1.v.Y*q2.v.Z - q1.v.Z*q2.v.Y,
							q1.n*q2.v.Y + q1.v.Y*q2.n + q1.v.Z*q2.v.X - q1.v.X*q2.v.Z,
							q1.n*q2.v.Z + q1.v.Z*q2.n + q1.v.X*q2.v.Y - q1.v.Y*q2.v.X);							
}
 
	Quaternion operator*(Quaternion q, float s)
{
	return	Quaternion(q.n*s, q.v.X*s, q.v.Y*s, q.v.Z*s);
}

	Quaternion operator*(float s, Quaternion q)
{
	return	Quaternion(q.n*s, q.v.X*s, q.v.Y*s, q.v.Z*s);
}

	Quaternion operator*(Quaternion q, vector3df v)
{
	return	Quaternion(	-(q.v.X*v.X + q.v.Y*v.Y + q.v.Z*v.Z),
							q.n*v.X + q.v.Y*v.Z - q.v.Z*v.Y,
							q.n*v.Y + q.v.Z*v.X - q.v.X*v.Z,
							q.n*v.Z + q.v.X*v.Y - q.v.Y*v.X);
}

	Quaternion operator*(vector3df v, Quaternion q)
{
	return	Quaternion(	-(q.v.X*v.X + q.v.Y*v.Y + q.v.Z*v.Z),
							q.n*v.X + q.v.Z*v.Y - q.v.Y*v.Z,
							q.n*v.Y + q.v.X*v.Z - q.v.Z*v.X,
							q.n*v.Z + q.v.Y*v.X - q.v.X*v.Y);
}

	Quaternion operator/(Quaternion q, float s)
{
	return	Quaternion(q.n/s, q.v.X/s, q.v.Y/s, q.v.Z/s);
}

	float QGetAngle(Quaternion q)
{
	return	(float) (2*acos(q.n));
}

	vector3df QGetAxis(Quaternion q)
{
	vector3df v;
	float m;

	v = q.Getvector3df();
	m = (float) v.getLength();
	
	if (m <= tol)
		return vector3df();
	else
		return v/m;	
}

	Quaternion QRotate(Quaternion q1, Quaternion q2)
{
	return	q1*q2*(~q1);
}

	vector3df	QVRotate(Quaternion q, vector3df v)
{
	Quaternion t;


	t = q*v*(~q);

	return	t.Getvector3df();
}

	Quaternion	MakeQFromEulerAngles(float x, float y, float z)
{
	Quaternion	q;
	double	roll = DegreesToRadians(x);
	double	pitch = DegreesToRadians(y);
	double	yaw = DegreesToRadians(z);
	
	double	cyaw, cpitch, croll, syaw, spitch, sroll;
	double	cyawcpitch, syawspitch, cyawspitch, syawcpitch;

	cyaw = cos(0.5f * yaw);
	cpitch = cos(0.5f * pitch);
	croll = cos(0.5f * roll);
	syaw = sin(0.5f * yaw);
	spitch = sin(0.5f * pitch);
	sroll = sin(0.5f * roll);

	cyawcpitch = cyaw*cpitch;
	syawspitch = syaw*spitch;
	cyawspitch = cyaw*spitch;
	syawcpitch = syaw*cpitch;

	q.n = (float) (cyawcpitch * croll + syawspitch * sroll);
	q.v.X = (float) (cyawcpitch * sroll - syawspitch * croll); 
	q.v.Y = (float) (cyawspitch * croll + syawcpitch * sroll);
	q.v.Z = (float) (syawcpitch * croll - cyawspitch * sroll);

	return q;
}

	vector3df	MakeEulerAnglesFromQ(Quaternion q)
{
	double	r11, r21, r31, r32, r33, r12, r13;
	double	q00, q11, q22, q33;
	double	tmp;
	vector3df	u;

	q00 = q.n * q.n;
	q11 = q.v.X * q.v.X;
	q22 = q.v.Y * q.v.Y;
	q33 = q.v.Z * q.v.Z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (q.v.X*q.v.Y + q.n*q.v.Z);
	r31 = 2 * (q.v.X*q.v.Z - q.n*q.v.Y);
	r32 = 2 * (q.v.Y*q.v.Z + q.n*q.v.X);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if(tmp > 0.999999)
	{
		r12 = 2 * (q.v.X*q.v.Y - q.n*q.v.Z);
		r13 = 2 * (q.v.X*q.v.Z + q.n*q.v.Y);

		u.X = RadiansToDegrees(0.0f); //roll
		u.Y = RadiansToDegrees((float) (-(pi/2) * r31/tmp)); // pitch
		u.Z = RadiansToDegrees((float) atan2(-r12, -r31*r13)); // yaw
		return u;
	}

	u.X = RadiansToDegrees((float) atan2(r32, r33)); // roll
	u.Y = RadiansToDegrees((float) asin(-r31));		 // pitch
	u.Z = RadiansToDegrees((float) atan2(r21, r11)); // yaw
	return u;
	







//------------------------------------------------------------------------//
// Misc. Functions
//------------------------------------------------------------------------//




}


