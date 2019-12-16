@ECHO OFF
CLS

IF NOT "%1" == "" GOTO dragndrop

ECHO Horde3D Collada Converter Batch
ECHO.
ECHO Arguments:
ECHO inputFile [-o outputName] [-s shaderName] [-opt]
ECHO.
ECHO inputFile:      filename of the COLLADA document
ECHO -o outputName:  name of the output files (without extension)
ECHO -s shaderName:  filename of the default shader for materials
ECHO -noopt:         disable geometry optimization
ECHO -anim:          export animations only
ECHO.

SET cmdLine=
SET /P cmdLine=Enter arguments: 
ECHO.
ECHO Starting Collada Converter...
ECHO.

ColladaConv %cmdLine%

ECHO.
GOTO exit


:dragndrop

%~d0 
cd %~dp0
ColladaConv %1


:exit

PAUSE
