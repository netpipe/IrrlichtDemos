#include <string>

bool mapLoaded = false;
unsigned x_clip,y_clip;

struct map{
    
};   
 
void initializeMapSystem(){
}



void destroyMap(){
}

void loadMap(){ 

//    	Distroy_Old_map()
    destroyMap();
    
//	If maploaded = True Then RuntimeError("Failed to remove old map")

    
//Open some sort of file...

//	Local usetile
//	Local gameTile
//	reset_tiles()
//	If level_piv <> 0 Then FreeEntity level_piv
//	level_piv = CreatePivot()
// 	Restore samplemap
//	Local filehandle = ReadFile(FiletoLoad$)
//	If filehandle = 0 Then Return 0
//	
    mapLoaded = true;
//
//	map_name$ = ReadString$(filehandle)
//	map_disc$ = ReadString$(filehandle)
//	GOpenMap$ = parse$(FiletoLoad$,"\",parsecount(FiletoLoad$,"\",0),0)
//	usetile = ReadInt(filehandle)
//	Map_TileSet = usetile
//;	Map_Tileset = 1
//;	usetile = 1
//	prime_tileset(usetile)
//	x_clip = ReadInt(filehandle)
//	y_clip = ReadInt(filehandle)
//	Dim map_data(x_clip,y_clip)
//	Dim game_array(x_clip,y_clip)
//	Dim ref_data(x_clip,y_clip)
//	;im Game_Data_Array(X_clip,Y_clip)
//	
    for(unsigned i=0;i < y_clip; i++)
        {
            for(unsigned j=0; j < x_clip; j++)
            {
//			gameTile  = ReadInt(filehandle)
//			;ame_Data_Array(j,i) = gametile 
//			ref_data(j,i) = gametile ;So you dont have to reload the map every new round
//			map_data(j,i) = create_tile(gameTile,j,i) ;3d pointer
//			;EntityParent(map_data(j,i),level_piv)
           }    
        }        
//	;PowerUps/Special
//	Local pu_num = ReadInt(filehandle)
//	Local pu_flag
//	Local pu_x
//	Local pu_y
//	For i = 1 To pu_num
//		pu_flag = ReadInt(filehandle)
//		pu_x = ReadInt(filehandle)
//		pu_y = ReadInt(filehandle)
//		spawn_pu(pu_flag,pu_x,pu_y)
//	Next
//	If GameType = 1 spawn_ffa(1)
//	;
//	CloseFile(filehandle)

}

void refreshMap(){
}
    
            
