// The code is distributed under the GNU GPL v3
// Part of the OSRPG project: http://osrpgcreation.sf.net
// Do you want to create the best RPG? Join us!
// Copyright (C) 2006 Michael Kapelko (aka kornerr)

#include "sky.h"
using namespace irr;
using namespace core;

MapSky::MapSky (Registry *rg) {
	reg = rg;
	tex_dir = reg->settings->GetDataDir () + "textures/";
	skydome_is_used = true;
	n = 0;
	Clear ();
}

MapSky::~MapSky () {
	Clear ();
}

void MapSky::Clear () {
	if (n) {
		n->remove ();
		n = 0;
	}
	memset (&sb, 0, sizeof (OMFSkyBox));
	memset (&sd, 0, sizeof (OMFSkyDome));
}

void MapSky::CreateNode () {
	n = 0;
	if (skydome_is_used)
		SetSkyDome (sd);
	else
		SetSkyBox (sb);
}

OMFSkyBox MapSky::GetSkyBoxCopy () const {
	return sb;
}

OMFSkyDome MapSky::GetSkyDomeCopy () const {
	return sd;
}

void MapSky::SetSkyBox (OMFSkyBox sb) {
	Clear ();
	stringc s1,
			s2,
			s3,
			s4,
			s5,
			s6;
	s1 = tex_dir + sb.top;
	s2 = tex_dir + sb.bottom;
	s3 = tex_dir + sb.left;
	s4 = tex_dir + sb.right;
	s5 = tex_dir + sb.front;
	s6 = tex_dir + sb.back;
	n = reg->smgr->addSkyBoxSceneNode (
			reg->drv->getTexture (s1.c_str ()),
			reg->drv->getTexture (s2.c_str ()),
			reg->drv->getTexture (s3.c_str ()),
			reg->drv->getTexture (s4.c_str ()),
			reg->drv->getTexture (s5.c_str ()),
			reg->drv->getTexture (s6.c_str ()));
	skydome_is_used = false;
	MapSky::sb = sb;
}

void MapSky::SetSkyDome (OMFSkyDome sd) {
	Clear ();
	stringc s = tex_dir + sd.texture;
	n = reg->smgr->addSkyDomeSceneNode (reg->drv->getTexture (s.c_str ()),
			sd.horiz_res, sd.vert_res, sd.tpercentage, sd.spercentage);
	skydome_is_used = true;
	MapSky::sd = sd;
}

bool MapSky::SkyDomeIsUsed () const {
	return skydome_is_used;
}

void MapSky::SetSkyDomeUsed (bool state) {
	Clear ();
	skydome_is_used = state;
}

