
class SimplexNoise1234
{
	public:
		SimplexNoise1234() {}
		~SimplexNoise1234() {}
		static float noise ( float x, float y );
		static float pnoise ( float x, float y, int px, int py );
	private:
		static unsigned char perm[];
		static float  grad ( int hash, float x, float y );
};

