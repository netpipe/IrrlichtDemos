#!BPY    

"""
Name: 'HORDE3D (.xml) ...'
Blender: 246
Group: 'Export'
Tooltip: 'Export scene from Blender to Horde3D format (.xml)'
"""

__author__ = "Felix Kistler and Marcel Pham"
__url__ = ("Uni Augsburg, http://mm-werkstatt.informatik.uni-augsburg.de")
__version__ = "0.2"
__email__ = "felixkistler@web.de"
__bpydoc__ = """\

Description: Exports a Blender scene into a Horde3D file.

Usage: Run the script from the menu or inside Blender. 
"""

_ERROR = False

try:
    import Blender
    from Blender import *
    from Blender.Draw import *
    from Blender.BGL import *
    from Blender.Mathutils import *
except:
    print "\nError! Could not find Blender module!"
    _ERROR = True

try:
    import os
    import struct
except:
    print "\nError! Could not find IO modules!"
    _ERROR = True

if _ERROR:
    print """Could not load all modules! Please install Python from http://www.python.org
        and the newest blender version from http://www.blender.org!"""
    Blender.Draw.PupMenu("Could not load all modules! Check console!")


# Events
EVENT_NOEVENT = 1
EVENT_EXPORT  = 2
EVENT_EXIT    = 3
EVENT_BROWSE  = 4
EVENT_BUTTONCHANGE = 5

# Polygon Parameters
fileButton = Create('')
shaderButton = Create('')
textureButton = Create('')
materialButton = Create('')
animationButton = Create('')
texPathButton = Create('')
hordeReg = Blender.Registry.GetKey('horde',True)

        
######################################################
# GUI
######################################################
def draw():
    if not _ERROR:
        global fileButton, shaderButton, textureButton, materialButton, animationButton, texPathButton
        global hordeReg
        global EVENT_NOEVENT, EVENT_EXPORT, EVENT_EXIT, EVENT_BROWSE, EVENT_BUTTONCHANGE

        size = Blender.Window.GetAreaSize()
        # Titel
        glClearColor(0.7,0.8,0.6,1)
        glClear(GL_COLOR_BUFFER_BIT)
        Blender.BGL.glColor3f(0.2, 0.3, 0.7)
        Blender.BGL.glRasterPos2d(8, size[1]-20)
        Blender.Draw.Text("Felix Horde3D Exporter", "large")

        # Die Parameter der GUI Buttons
        glRasterPos2d(8, 83)
        fileName = fileButton.val
        if (fileName==''):
            fileName = Blender.sys.dirname(Blender.sys.progname)+Blender.sys.sep+'untitled.scene.xml'
            if not (hordeReg is None) and 'path' in hordeReg:
                fileName = hordeReg['path']
            
        shaderName = shaderButton.val
        if (shaderName==''):
            if not (hordeReg is None) and 'shader' in hordeReg:
                shaderName = hordeReg['shader']
            else:
                shaderName = 'skinning.shader.xml'
                
        texSubpath = textureButton.val
        if (texSubpath==''):
            if not (hordeReg is None) and 'texSub' in hordeReg:
                texSubpath = hordeReg['texSub']
                
        matPath = materialButton.val
        if (matPath == ''):
            if not (hordeReg is None) and 'matPath' in hordeReg:
                matPath = hordeReg['matPath']
            else:
                matPath = '..'+Blender.sys.sep+'materials'
                
        animPath = animationButton.val
        if (animPath == ''):
            if not (hordeReg is None) and 'animPath' in hordeReg:
                animPath = hordeReg['animPath']
            else:
                animPath = '..'+Blender.sys.sep+'animations'
                
        texPath = texPathButton.val
        if (texPath == ''):
            if not (hordeReg is None) and 'texPath' in hordeReg:
                texPath = hordeReg['texPath']
                
        Label('Export Filename:', 10, size[1]/2+100, 100, 20)
        fileButton = String('', EVENT_BUTTONCHANGE, 210, size[1]/2+100, 400, 20, fileName, 255, 'Filename')
        Button('...', EVENT_BROWSE, 610, size[1]/2+100, 30, 20)
        
        Label('Shader:', 10, size[1]/2+60, 100, 20)
        shaderButton = String('', EVENT_BUTTONCHANGE, 210, size[1]/2+60, 200, 20, shaderName, 255, 'Shader')
        
        Label('Relative Material Path:', 10, size[1]/2+20, 200, 20)
        materialButton = String('', EVENT_BUTTONCHANGE, 210, size[1]/2+20, 200, 20, matPath, 255, 'Material Path')
        
        Label('Relative Animation Path:', 10, size[1]/2-20, 200, 20)
        animationButton = String('', EVENT_BUTTONCHANGE, 210, size[1]/2-20, 200, 20, animPath, 255, 'Animation Path')
        
        Label('Texture SubDirectory:', 10, size[1]/2-60, 200, 20)
        textureButton = String('', EVENT_BUTTONCHANGE, 210, size[1]/2-60, 200, 20, texSubpath, 255, 'Texture SubDirectory')
        
        Label('Relative Texture Path:', 10, size[1]/2-100, 200, 20)
        texPathButton = String('', EVENT_BUTTONCHANGE, 210, size[1]/2-100, 200, 20, texPath, 255, 'Texture Path')
        Label('(If you want to copy textures)', 10, size[1]/2-115, 200,20)
        
        

        # Export- und Exitbuttons
        Button('Export',EVENT_EXPORT, 10, 10, 80, 18)
        Button('Cancel',EVENT_EXIT , 140, 10, 80, 18)

######################################################
# Event Handler
######################################################
def event(evt, val):       
    if (evt == QKEY and not val):
            Exit()
def bevent(evt):
    global fileButton, shaderButton, textureButton, materialButton, animationButton, texPathButton
    global hordeReg
    global EVENT_NOEVENT, EVENT_EXPORT, EVENT_EXIT, EVENT_BROWSE, EVENT_BUTTONCHANGE

    # GUI Events behandeln
    if (evt == EVENT_BUTTONCHANGE):
        file = fileButton.val
        shader = shaderButton.val
        if shader =='':
            shader = 'skinning.shader.xml'
        texSub = textureButton.val
        matPath = materialButton.val
        texPath = texPathButton.val
        animPath = animationButton.val
        
        d = {}
        d['path']=file
        d['shader']=shader
        d['texSub']=texSub
        d['texPath']=texPath
        d['animPath']=animPath
        d['matPath']=matPath
        Blender.Registry.SetKey('horde', d, True)
        
    if (evt == EVENT_EXIT):
        Exit()
            
    elif (evt== EVENT_BROWSE):
        Blender.Window.FileSelector(FileSelected,'Select File', Blender.sys.basename(fileButton.val))
        Redraw(1)
            
    elif (evt== EVENT_EXPORT):
        file = fileButton.val
        shader = shaderButton.val
        if shader =='':
            shader = 'skinning.shader.xml'
        texSub = textureButton.val
        matPath = materialButton.val
        texPath = texPathButton.val
        animPath = animationButton.val
        
        d = {}
        d['path']=file
        d['shader']=shader
        d['texSub']=texSub
        d['texPath']=texPath
        d['animPath']=animPath
        d['matPath']=matPath
        Blender.Registry.SetKey('horde', d, True)
        
        startTime = Blender.sys.time()
        success = Export(file, shader, texSub, texPath, matPath, animPath)
        endTime = Blender.sys.time()
        exportTime = endTime - startTime
        
        if success:
            print "Export took: %.6f seconds" % (exportTime )
            Blender.Draw.PupMenu('Export successful!')
        else:
            print "Export failed!"
            Blender.Draw.PupMenu('Export failed, see Console!')
        

# Register GUI and Event functions
Register(draw, event, bevent)

######################################################
# Callback for File Selector
######################################################
def FileSelected(file):
    global fileButton
    fileButton.val = file
    Redraw(1)
    
######################################################
# Export Main
######################################################
def Export(path, defShader, texturesub, texPath, matPath, animPath):
    
    # Show the wait cursor in blender and stop editing mode
    Blender.Window.WaitCursor(1)
    Blender.Window.EditMode(0)
    
    # Get full path, extract directory name and file name without extension
    dir = Blender.sys.dirname(path) + Blender.sys.sep
    fileName = Blender.sys.basename(path)
    fileName = fileName.partition(".")[0]
    # Get texture subpath
    texSubPath = ''
    if texturesub != '':
        texSubPath = texturesub + Blender.sys.sep
    # Get absolute pathes
    if texPath != '':
        texPath = dir+texPath+Blender.sys.sep+texSubPath
    if matPath != '':
        matPath = matPath+Blender.sys.sep
    matPath = dir+matPath
    if animPath != '':
        animPath = animPath + Blender.sys.sep
    animPath = dir+animPath
    
    # Check if File and Directory exist
    if Blender.sys.exists(path):
        if not Blender.Draw.PupMenu( "File Already Exists, Overwrite?%t|Yes%x1|No%x0" ):
            print "Aborted by User!"
            return False
    if not Blender.sys.exists(dir):
        Blender.Draw.PupMenu( "Path is invalid!" )
        return False
        
    print "Exporting %s ..." % path
    
    # Change working directory
    try: os.chdir(dir)
    except: 
        print "Couldn't change working directory!"
        return False
    
    # Get current scene and objects
    scn = Scene.GetCurrent()
    
    converter = Converter(path, texSubPath, texPath, matPath, animPath)
    
    print "Converting Model..."
    if converter.convertModel( scn ) == False:
        return False
    print "Done."
    
    print "Writing geometry..." 
    if converter.saveModel( fileName ) == False:
        return False
    print "Done."
        
    print "Writing materials..."
    if converter.writeMaterials( fileName, defShader ) == False:
        return False
    print "Done."

    print "Writing animation..."
    if converter.writeAnimation( fileName ) == False:
        return False
    print "Done."
    
    if texPath != '':
        print "Copying textures..."
        if converter.copyTextures() == False:
            return False
        print "Done."
        
    # Hide the wait cursor in blender
    Blender.Window.WaitCursor(0)
    return True
    
######################################################
# Helper Classes to save model data
######################################################
class Vertex():
    def __init__(self, pos, normal, uv, index, bIndex):
        self.pos = pos
        self.storedNormal = normal
        self.posIndex = index
        self.bIndex = bIndex
        self.normal = Vector()
        self.tangent = Vector([0,0,0])
        self.bitangent = Vector([0,0,0])
        self.texCoords = uv
        self.joints = []
        self.weights = []

class TmpVertex():
    def __init__(self, globIdx, bIdx, bFaceIdx, bNors, uvs): 
        self.globIdx = globIdx
        self.bIdx = bIdx
        self.bFaceIdx = bFaceIdx
        self.bNors = bNors
        self.nors = Vector([0,0,0]) 
        self.uvs = uvs
        self.tans = Vector([0,0,0])
        self.bitans = Vector([0,0,0])
        self.diff = True
        self.diffPos = Vector()
         
class TriGroup():
    def __init__(self, first, vStart):
        self.first, self.count = first,0
        self.vertRStart, self.vertREnd = vStart,0 
        self.material = None
        self.posIndexToVertices = []

class HordeMesh():
    def __init__(self, name, bName, matRel, matAbs):
        self.triGroups = []
        self.bName = bName
        self.matRel = matRel
        self.matAbs = matAbs
        self.material = None
        self.batchStart = 0
        self.batchCount = 0
        self.parent = ''
        self.children = []
        self.frames = []
        if len(name) < 255:
            self.name = name
        else:
            self.name = name[:255]
        self.influences = {}


class HordeJoint():
    def __init__(self, name, parent, matAbs, matRel, index):
        self.used = False
        self.index = index
        self.name  = name
        self.parent = parent
        self.children = []
        self.frames = []
        self.matAbs = matAbs
        self.matRel = matRel
        self.invBindMat = Matrix(matAbs).invert()

class MorphDiff():
    def __init__(self, index, pos, normal, tangent, bitangent):
        self.vertIndex = index
        self.pos = pos
        self.normal = normal
        self.tangent = tangent
        self.bitangent = bitangent

class MorphTarget():
    def __init__(self, name):
        self.diffs = []
        if len(name) < 255:
            self.name = name
        else:
            self.name = name[:255]

######################################################
# Converter Class used for reading, converting and saving the scene
######################################################
class Converter():
    
    def __init__(self, path, sub, texPath, matPath, animPath):
        self.__vertices = []
        self.__meshes = []
        self.__joints = []
        self.__morphTargets = []
        self.__frameCount = 0
        self.__animCount = 0
        self.__indices = []
        
        self.__bObjects = []
        self.__materials = {}
        self.__filePath = path
        self.__texSubPath = sub
        self.__texPath = texPath
        self.__matPath = matPath
        self.__animPath = animPath
        self.__images = []
        self.__AdditionalJointMat = Matrix()
        
    def convertModel(self, scn):
        ##################
        # Load and parse model data from blender
        ##################
        
        # Set to first frame and remember current
        context = scn.getRenderingContext()
        originalFrame = Blender.Get('curframe')
        Blender.Set('curframe', context.startFrame())
        
        self.__bObjects = list(scn.objects)
        
        # Get additional joint matrix
        for obj in self.__bObjects:
            if obj.getParent() != None and obj.getParent().getType() == 'Armature':
                self.__AdditionalJointMat = Matrix(obj.mat).invert()
                break
        
        # Get Joints
        jCount = 0
        for obj in self.__bObjects:
            if obj.getType() == 'Armature':
                # Import Armatures
                arm = obj.getData(False, True)
                # Get Bones
                for bone in arm.bones.values():                    
                    # Get joint bind pose
                    matAbs = bone.matrix['ARMATURESPACE'] * obj.mat * self.__AdditionalJointMat
                    if bone.hasParent():
                        parent = bone.parent.name
                        parentMat = bone.parent.matrix['ARMATURESPACE'] * obj.mat * self.__AdditionalJointMat
                        matRel = matAbs * parentMat.invert()
                    else:
                        parent = ''
                        matRel = Matrix(matAbs)
                    jCount += 1
                    joint = HordeJoint( bone.name, parent, matAbs, matRel, jCount)
                    self.__joints.append(joint)
                    
            elif obj.getType() != 'Mesh':
                print "Warning: Can't import non-mesh and non-armature object: %s!" % obj.getName()
        
        # Get Meshes
        vCount = 0
        for obj in self.__bObjects:
            if obj.getType() == 'Mesh':
                
                # Select active object and apply modifiers
                scn.objects.selected = []
                obj.sel = 1
                obj.activeShape = 1
                obj.pinShape = True
                # Duplicate linked
                Object.Duplicate()
                tempOB = scn.objects.active
                # Needed to apply the modifier
                tempOB.makeDisplayList()
                Window.RedrawAll()
                mesh = Mesh.New()
                mesh.getFromObject(tempOB,0,0)
                tempOB.link(mesh)
                    
                # Import Mesh
                if obj.getParent() != None and obj.getParent().getType() == 'Mesh':
                    newMesh = HordeMesh( obj.getName(), obj.name, Matrix(tempOB.matrixLocal), Matrix(tempOB.mat))
                    newMesh.parent = obj.getParent().name
                elif obj.getParent() != None and obj.getParent().getType() == 'Armature':
                    # Object matrix will already be imported by joints
                    newMesh = HordeMesh( obj.getName(), obj.name, Matrix(), Matrix())
                else:
                    newMesh = HordeMesh( obj.getName(), obj.name, Matrix(tempOB.mat), Matrix(tempOB.mat))
                    
                # Get vertex influences by index
                for v in mesh.verts:
                    influences = mesh.getVertexInfluences(v.index)
                    if influences != None:
                        newMesh.influences[v.index] = influences
                        
                # Import Materials                        
                for mat in mesh.materials:
                    # Don't create new material if it is already in list
                    if mat.name in self.__materials:
                        continue    
                    material = {}
                    material['name'] = mat.getName()
                    i = 0
                    while material['name'] in self.__materials:
                        material['name'] = mat.getName() + str(i)
                        i += 1
                    material['textures'] = []
                    i = 0
                    for texture in mat.getTextures():
                        if i > 10:
                            print "Warning: Too many textures in material %s!" % material['name']
                            break
                        texName = ''
                        if texture != None and texture.tex != None and texture.tex.type == Texture.Types.IMAGE and texture.tex.image != None:
                            texName = self.__texSubPath+Blender.sys.basename(texture.tex.image.filename)
                            texName = texName.replace('\\','/')
                            material['textures'].append( (i, texName) )
                            if texture.tex.image not in self.__images:
                                self.__images.append(texture.tex.image)
                            i += 1
                    # If material has no textures
                    # try to import a uniform with color infos
                    material['uniform'] = None        
                    if i == 0:        
                        material['uniform'] = [round(c, 5) for c in mat.getRGBCol()]
                        material['uniform'].append(round(mat.getAlpha(), 5))
                    self.__materials[mat.name] = material
                                        
                shapes = []
                if not obj.data.key == None:
                    keys = range(len(obj.data.key.blocks))
                else:
                    keys = [0]
                
                # Check for quads
                hasQuads = False
                for face in mesh.faces:
                    if len(face) == 4:
                        hasQuads = True
                        break
        
                for i in keys:
                    # i == 0 is unmorphed mesh
                    if i > 0:
                        # Delete last key
                        scn.objects.unlink(tempOB)
                        # get current morph target
                        obj.activeShape = i+1
                        obj.pinShape = True
                        # Select active object and apply modifiers
                        scn.objects.selected = []
                        obj.sel = 1
                        # Duplicate linked
                        Object.Duplicate()
                        tempOB = scn.objects.active
                        # Needed to apply the modifier
                        tempOB.makeDisplayList()
                        Window.RedrawAll()
                        mesh = Mesh.New()
                        mesh.getFromObject(tempOB,0,0)
                        tempOB.link(mesh)
                        
                    # Convert Quads to triangles if nescessary
                    if hasQuads:
                        oldmode = Mesh.Mode()
                        Mesh.Mode(Mesh.SelectModes['FACE'])
                        mesh.sel = True
                        tempob = scn.objects.new(mesh)
                        mesh.quadToTriangle(0)
                        oldmode = Mesh.Mode(oldmode)
                        scn.objects.unlink(tempob)
                        Mesh.Mode(oldmode)
                        
                    # Get shapekey's name
                    if not obj.data.key == None:
                        name = obj.data.key.blocks[i].name
                    else:
                        name = ''
                    shapes.append([mesh,name] )
                
                vCount = self.calculateTriGroups( shapes, newMesh, vCount)
                # Only append Mesh if it has triangles
                if len(newMesh.triGroups) > 0:
                    self.__meshes.append(newMesh)
         
                # Convert joint influences for vertices
                for tri in newMesh.triGroups:
                    for v in xrange(tri.vertRStart, tri.vertREnd+1):
                        influences = newMesh.influences[self.__vertices[v].bIndex]
                        if influences == None or len(influences) == 0:
                            continue
                        # Sort influences by weight desc if more than 4
                        # because Horde only wants 4 of them
                        if len(influences) > 4:
                            # Copy only if we need to change them
                            influences = influences[:]
                            influences.sort(key = lambda x: x[1], reverse=True)
                        for name, weight in influences:
                            for joint in self.__joints:
                                if joint.name == name:
                                    self.__vertices[v].joints.append(joint)
                                    self.__vertices[v].weights.append(weight)
                                    break
                
                # Delete object with applied modifiers
                scn.objects.unlink(tempOB)

        
        ##################
        # Convert Model
        ##################
        
        # Animations
        self.processFrames(context)
        
        # Process joints and meshes
        self.processJoints()
        self.processMeshes()
                
        # Calculate Number of objects with frames
        obCount = 0
        for mesh in self.__meshes:
            if len(mesh.frames) > 0:
                self.__animCount += 1        
        m = self.__animCount
        if m > 0:
            print "  Added %d Mesh-Animation(s)..." % m
        
        for joint in self.__joints:
            if len(joint.frames) > 0:
                self.__animCount += 1
        
        if (self.__animCount - m) > 0:
            print "  Added %d Joint-Animation(s)..." % (self.__animCount - m)
            
        
        # Move back to original view
        Blender.Set('curframe', originalFrame)
        
        return True
    
    def calculateTriGroups( self, shapes, newMesh, vCount):
        mesh = shapes[0][0]
        glob2Loc = {}
        loc2Data = []
                
        faces = [[f, False] for f in mesh.faces]
        for f in faces:
            if f[1] == True:
                continue
                        
            t = TriGroup(len(self.__indices), vCount)
            
            if f[0].mat < len(mesh.materials):
                t.material = mesh.materials[f[0].mat].name
                if newMesh.material == None:
                    newMesh.material = t.material
                
            # Append all triangles with same material
            f2Idx = -1;
            for f2 in faces:
                f2Idx += 1
                if f2[1] == True:
                    continue
                
                # Check for same material
                matName = None
                if f2[0].mat < len(mesh.materials):
                    matName = mesh.materials[f2[0].mat].name
                if matName != t.material:
                    continue
                    
                f2[1] = True
                
                # append vertices if not already in list
                vIndex = 0
                if not f2[0].smooth:
                    normal = Vector(f2[0].no)

                for vert in f2[0].verts:
                    # Find uv coordinates
                    if mesh.faceUV and len(f2[0].uv) > vIndex:
                        uv = f2[0].uv[vIndex]
                    else:
                        uv = [0,0]    
                    if mesh.vertexUV:
                        uv[0] += vert.uvco[0]
                        uv[1] += vert.uvco[1]
                        
                    # If smooth use vertex normals
                    if f2[0].smooth:
                        normal = Vector(vert.no)
                    
                    found = False
                    pindex = -1
                    pi = 0
                    vertCo = Vector(vert.co)
                    for posindex in t.posIndexToVertices:
                        if posindex[0] == vert.index:
                            pindex = pi
                            break
                        pi += 1
                    
                    if pindex != -1:
                        pi = 1
                        while( pi < len(t.posIndexToVertices[pindex])):
                            index = t.posIndexToVertices[pindex][pi]
                            vertice = self.__vertices[index]
                            if vertice.texCoords == uv and normal == vertice.storedNormal:
                                found = True
                                break
                            pi += 1
                    
                    vlen = len(self.__vertices)                    
                    if found == True:
                        self.__indices.append(index)
                        t.count += 1
                    elif pindex != -1:
                        # Position found but no equal vertices -> append new vertex in position list
                        self.__indices.append(vlen)
                        t.posIndexToVertices[pindex].append(vlen)
                        t.count += 1
                        self.__vertices.append( Vertex(vertCo, normal, uv, pindex, vert.index) )
                        glob2Loc[vCount] = len(loc2Data)
                        loc2Data.append( TmpVertex(vCount, vert.index, f2Idx, normal, uv) )
                        vCount += 1
                    else:
                        # Position not found -> append new vertex in new position list
                        self.__indices.append(vlen)
                        t.posIndexToVertices.append([vert.index, vlen])
                        t.count += 1
                        self.__vertices.append( Vertex(vertCo, normal, uv, len(t.posIndexToVertices)-1, vert.index) )
                        glob2Loc[vCount] = len(loc2Data)
                        loc2Data.append( TmpVertex(vCount, vert.index, f2Idx, normal, uv) )
                        vCount += 1
                    vIndex += 1
                                    
            t.vertREnd = vCount-1
            print "  Added Mesh with " + str(vCount - t.vertRStart) + " Vertices..."
            newMesh.triGroups.append(t)
            
        self.calcTangentSpaceBasis( newMesh, mesh, glob2Loc, loc2Data );
        for v in loc2Data:
            vertex = self.__vertices[v.globIdx]
            vertex.normal = v.nors
            v.nors = Vector([0,0,0])
            vertex.tangent = v.tans
            v.tans = Vector([0,0,0])
            vertex.bitangent = v.bitans  
            v.bitans = Vector([0,0,0])
            
        # Get morph targets
        for shape in shapes[1:]:
                      
            target = MorphTarget( shape[1] )
            
            for v in loc2Data:
                vert = shape[0].verts[v.bIdx]
                pos = vert.co - mesh.verts[v.bIdx].co
                face = shape[0].faces[v.bFaceIdx]
                    
                if not face.smooth:
                    normal = Vector(face.no)
                    noDiff = mesh.faces[v.bFaceIdx].no - face.no
                else:
                    normal = Vector(vert.no)
                    noDiff = mesh.verts[v.bIdx].no - vert.no
                                        
                if pos[0] == 0 and pos[1]==0 and pos[2]==0 and noDiff[0] == 0 and noDiff[1]==0 and noDiff[2]==0:
                    v.diff = False
                else:
                    v.diff = True
                    v.diffPos = Vector(pos)
                v.bNors = normal
                
            self.calcTangentSpaceBasis( newMesh, shape[0], glob2Loc, loc2Data );
            for v in loc2Data:
                if v.diff:
                    target.diffs.append( MorphDiff( v.globIdx, v.diffPos, v.nors, v.tans, v.bitans ) )
                v.nors = Vector([0,0,0])
                v.tans = Vector([0,0,0])
                v.bitans = Vector([0,0,0])
            
            self.__morphTargets.append( target )                     
            
        return vCount

    def processFrames(self, context):
        print "  Processing animations..."
        
        frames = xrange(context.startFrame(), context.endFrame()+1)
        self.__frameCount = len(frames)        
        
        jointAnims = False        
        
        # Joint animation
        if len(self.__joints) > 0:
            for obj in self.__bObjects:
                if obj.getType() == 'Armature':
                    pose = obj.getPose()
                    for frame in frames:
                        obj.evaluatePose(frame)    
                        for pbone in pose.bones.values():
                            # Get Bones
                            for joint in self.__joints:
                                if joint.name == pbone.name:
                                    matAbs = pbone.poseMatrix * obj.mat * self.__AdditionalJointMat
                                    if pbone.parent != None:
                                        parentMat = pbone.parent.poseMatrix * obj.mat * self.__AdditionalJointMat
                                        matRel = matAbs * parentMat.invert()
                                    else:
                                        matRel = Matrix(matAbs)
                                    joint.frames.append(matRel)
                                    # Set the joint matrices to first frame
                                    if frame == context.startFrame():
                                        joint.invBindMat = Matrix(matAbs).invert()
                                        joint.matAbs = matAbs
                                        joint.matRel = Matrix(matRel)
                                    jointAnims = True
                                    break                
        
        # Mesh animation
        # (only if there are no joint animations)
        if not jointAnims:
            for frame in frames:
                Blender.Set('curframe', frame)
                
                for obj in self.__bObjects:
                    if obj.getType() == 'Mesh':
                        # Find asscociated mesh
                        for mesh in self.__meshes:
                            if mesh.bName == obj.name:
                                # Import Data
                                mesh.frames.append(Matrix(obj.matrixLocal))
                                break
        
    def    processJoints(self ):
        print "  Calculating Joint dependencies..."
        if len(self.__joints) > 74:
            print "Warning: Horde only supports 74 joints"
            
        # Append children    
        for joint in self.__joints:    
            if joint.parent != "" and joint.parent != joint.name:
                for joint2 in self.__joints:
                    if joint2.name == joint.parent:
                        joint2.children.append(joint)
                        break
        
        # Recalculate joint indices
        jindex = 1
        for joint in self.__joints:
            if joint.parent == '':
                jindex = self.processJoint(joint, jindex)
                
        # Sort joints again by indices
        self.__joints.sort(key = lambda x: x.index)
                
    def processJoint(self, joint, jindex):
        joint.index = jindex
        jindex += 1
        for child in joint.children:
            jindex = self.processJoint(child, jindex)
        return jindex
        
    def    processMeshes( self ):
        print "  Calculating Mesh dependencies..."
        # TriGroup with different material = new submesh
        for mesh in self.__meshes:
            if not mesh.material == None:
                done = [mesh.material]
                oldTris = mesh.triGroups[:]
                for tri in oldTris:
                    if tri.material not in done:
                        triGroups = []
                        # Move trigroups
                        for tri2 in oldTris:
                            if tri2.material == tri.material:
                                triGroups.append(tri2)
                                mesh.triGroups.remove(tri2)
                        name = mesh.name
                        i = 0
                        # Create new name
                        while name in map(lambda v: v.name, self.__meshes):
                            name = mesh.name + str(i)
                            i += 1
                        if len(triGroups) != 0:
                            if len(mesh.triGroups) != 0:
                                newMesh = HordeMesh( name, '', Matrix(), mesh.matAbs)
                                newMesh.material = tri.material                        
                                newMesh.triGroups = triGroups
                                newMesh.parent = mesh.bName
                            else:
                                newMesh = HordeMesh( name, mesh.bName, mesh.matRel, mesh.matAbs)
                                newMesh.material = tri.material                        
                                newMesh.triGroups = triGroups
                                newMesh.parent = mesh.parent
                                for mesh2 in self.__meshes:
                                    if mesh2.parent == mesh.bName:
                                        mesh2.parent = mesh.parent
                                        mesh2.matRel = mesh.matRel
                            self.__meshes.append(newMesh)
                        else:
                            print "Warning: empty trigroup: " + name
                        done.append(tri.material)
        # Remove empty meshes
        # (should no longer be nesscecary)
        newMeshes = []
        removed = 0
        for m in range(len(self.__meshes)):
            if len(self.__meshes[m].triGroups) != 0:
                newMeshes.append(self.__meshes[m])
            else:
                removed += 1
        self.__meshes = newMeshes
        if removed > 0:
            print "  Warning: removed %d empty meshe(s)!" % removed
                        
        for mesh in self.__meshes:    
            # Find subMeshes and append children        
            if mesh.parent != "" and mesh.parent != mesh.bName:
                for mesh2 in self.__meshes:
                    if mesh2.bName == mesh.parent:
                        mesh2.children.append(mesh)
                        break
            # Calculate batchStart and batchCount
            mesh.batchStart = mesh.triGroups[0].first
            mesh.batchCount = mesh.triGroups[0].count
            
        # Process weights
        for vert in self.__vertices:
            l = len(vert.weights)
            if l > 4: 
                # Trim weights and joints
                vert.weights = vert.weights[:4]
                vert.joints = vert.joints[:4]
                # Make sure weights add up to 1
                weightSum = float(sum(vert.weights))
                for w in range(len(vert.weights)):
                    vert.weights[w] = vert.weights[w] / weightSum
            
        # Calculate tangent, bitangent and normal
        print "  Calculating tangents..."
        # self.calcTangentSpaceBasis()            
            
    def saveModel( self, fileName ):
        try:
            sceneFile = open(fileName+".scene.xml", "w")
        except:
            print "Error creating file: %s.scene.xml" % fileName
            return False
        try:
            geoFile = open(fileName+".geo", "wb")
        except:
            print "Error creating file: %s.geo" % fileName
            sceneFile.close()
            return False
        
        # Write model header
        sceneFile.write('<Model name="%s" geometry="%s.geo">\n' % (fileName, fileName) )
        
        # Write morph target names as comment
        if len(self.__morphTargets) > 0:
            sceneFile.write('\t<!-- Morph targets: ')
            for morph in self.__morphTargets:
                sceneFile.write('"'+morph.name+'" ')
            sceneFile.write("-->\n\n")
        
        # Write joint data
        for joint in self.__joints:
            if joint.parent == "":
                self.writeJoint(sceneFile, joint, 1)
                
        sceneFile.write('\n')
        
        # Write mesh data
        for mesh in self.__meshes:
            if mesh.parent == "":
                self.writeMesh(sceneFile, mesh, fileName, 1)
        
        # Close Scene file
        sceneFile.write('</Model>')
        sceneFile.close()
        
        # Write geo header
        geoFile.write('H3DG')
        version = struct.pack('i', 5)
        geoFile.write(version)
        
        # Write Number of Joints + 1 
        jointCount = struct.pack('i', len(self.__joints)+1)
        geoFile.write(jointCount)
        
        # Write identity Matrix
        identMat = struct.pack('16f', 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 )
        geoFile.write(identMat)
        
        # Write inversebind matrix for joints
        for joint in self.__joints:
            invBindMat = hordeMatrix(joint.invBindMat)
            for i in range(4):
                for j in range(4):
                    geoFile.write(struct.pack('f', round(invBindMat[i][j], 6)))
        
        # Write vertex stream data
        if len(self.__joints) == 0:
            count = struct.pack('i', 6)
        else:
            count = struct.pack('i', 8)
        geoFile.write(count)
        count = struct.pack('i', len(self.__vertices))
        geoFile.write(count)
        
        for i in range( 8 ):
            if( len(self.__joints) == 0 and (i == 4 or i == 5) ):
                continue
                
            # Position
            if i == 0:
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 3 * len( struct.pack('f', 0.0 ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:
                    pos = roundVec(v.pos)
                    geoFile.write( struct.pack('f', pos.x) )
                    geoFile.write( struct.pack('f', pos.y) )
                    geoFile.write( struct.pack('f', pos.z) )
            
            # Normal
            elif i == 1:
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 3 * len( struct.pack('h', 0 ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:
                    normal = roundVec(v.normal)
                    geoFile.write( struct.pack('h', normal.x*32767) )
                    geoFile.write( struct.pack('h', normal.y*32767) )
                    geoFile.write( struct.pack('h', normal.z*32767) )
            # Tangent
            elif i == 2:
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 3 * len( struct.pack('h', 0 ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:
                    tangent = roundVec(v.tangent)
                    geoFile.write( struct.pack('h', tangent.x*32767) )
                    geoFile.write( struct.pack('h', tangent.y*32767) )
                    geoFile.write( struct.pack('h', tangent.z*32767) )
            
            # Bitangent
            elif i == 3:
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 3 * len( struct.pack('h', 0 ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:                   
                    bitangent = roundVec(v.bitangent)                    
                    geoFile.write( struct.pack('h', bitangent.x*32767) )
                    geoFile.write( struct.pack('h', bitangent.y*32767) )
                    geoFile.write( struct.pack('h', bitangent.z*32767) )
                    
            
            # Joint indices
            elif i == 4:        
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 4 * len( struct.pack('c', 'c' ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:
                    jointIndices = [chr(0), chr(0), chr(0), chr(0)]
                    for j in range(len(v.joints)):
                        if j > 3:
                            break
                        jointIndices[j] = chr(v.joints[j].index)
                    
                    geoFile.write( struct.pack('c', jointIndices[0]) )
                    geoFile.write( struct.pack('c', jointIndices[1]) )
                    geoFile.write( struct.pack('c', jointIndices[2]) )
                    geoFile.write( struct.pack('c', jointIndices[3]) )
            
            # Weights
            elif i == 5:
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 4 * len( struct.pack('c', 'c' ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:
                    weights = [1, 0, 0, 0]
                    for w in range(len(v.weights)):
                        if w > 3:
                            break
                        weights[w] = v.weights[w]    
                    geoFile.write( struct.pack('c', chr(int(round(weights[0] * 255)))) )
                    geoFile.write( struct.pack('c', chr(int(round(weights[1] * 255)))) )
                    geoFile.write( struct.pack('c', chr(int(round(weights[2] * 255)))) )
                    geoFile.write( struct.pack('c', chr(int(round(weights[3] * 255)))) )

            # Texture Coord Set 1
            elif i == 6:        
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 2 * len( struct.pack('f', 0.0 ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:
                    geoFile.write( struct.pack('f', v.texCoords[0]) )
                    geoFile.write( struct.pack('f', v.texCoords[1]) )

            # Texture Coord Set 2
            elif i == 7:
                geoFile.write( struct.pack('i', i ))
                streamElemSize = 2 * len( struct.pack('f', 0.0 ))
                geoFile.write( struct.pack('i', streamElemSize) )
                for v in self.__vertices:
                    geoFile.write( struct.pack('f', v.texCoords[0]) )
                    geoFile.write( struct.pack('f', v.texCoords[1]) )


        # Write triangle indices
        count = struct.pack('i', len(self.__indices))
        geoFile.write(count)

        for i in self.__indices:
            geoFile.write( struct.pack('i', i))
            
        # Write morph targets
        count = struct.pack('i', len(self.__morphTargets))
        geoFile.write(count)
        
        for morph in self.__morphTargets:
            geoFile.write(str256(morph.name));
            
            # Write vertex indices
            geoFile.write( struct.pack('i', len(morph.diffs)))

            # indices of the vertices which should be morphed
            for mDiff in morph.diffs:
                geoFile.write( struct.pack('i', mDiff.vertIndex))
            
            # Write stream data
            geoFile.write( struct.pack('i', 4))
            
            for i in range(4):
                # Position
                if i == 0:
                    geoFile.write( struct.pack('i', i ))
                    streamElemSize = 3 * len( struct.pack('f', 0.0 ))
                    geoFile.write( struct.pack('i', streamElemSize) )
                    for v in morph.diffs:
                        pos = roundVec(v.pos)
                        geoFile.write( struct.pack('f', pos.x) )
                        geoFile.write( struct.pack('f', pos.y) )
                        geoFile.write( struct.pack('f', pos.z) )
                
                # Normal
                elif i == 1:
                    geoFile.write( struct.pack('i', i ))
                    streamElemSize = 3 * len( struct.pack('f', 0.0 ))
                    geoFile.write( struct.pack('i', streamElemSize) )
                    for v in morph.diffs:
                        normal = roundVec(v.normal)
                        geoFile.write( struct.pack('f', normal.x) )
                        geoFile.write( struct.pack('f', normal.y) )
                        geoFile.write( struct.pack('f', normal.z) )
                # Tangent
                elif i == 2:
                    geoFile.write( struct.pack('i', i ))
                    streamElemSize = 3 * len( struct.pack('f', 0.0 ))
                    geoFile.write( struct.pack('i', streamElemSize) )
                    for v in morph.diffs:
                        tangent = roundVec(v.tangent)
                        geoFile.write( struct.pack('f', tangent.x) )
                        geoFile.write( struct.pack('f', tangent.y) )
                        geoFile.write( struct.pack('f', tangent.z) )
                
                # Bitangent
                elif i == 3:
                    geoFile.write( struct.pack('i', i ))
                    streamElemSize = 3 * len( struct.pack('f', 0.0 ))
                    geoFile.write( struct.pack('i', streamElemSize) )
                    for v in morph.diffs:
                        bitangent = roundVec(v.bitangent)
                        geoFile.write( struct.pack('f', bitangent.x) )
                        geoFile.write( struct.pack('f', bitangent.y) )
                        geoFile.write( struct.pack('f', bitangent.z) )
        
        # Close geo file
        geoFile.close()    
        return True
        
                    
    def writeMesh( self, sceneFile, mesh, fileName, numTabs):
        s = roundSize(mesh.matRel.scalePart())
        r = roundVec(mesh.matRel.toEuler())
        t = roundVec(mesh.matRel.translationPart())
        mat = ' '
        if mesh.material != None:
            mat = fileName+'/'+self.__materials[mesh.material]['name']+".material.xml"
        else:
            print "Warning: No material found for mesh: "+mesh.name+"!"
            
        # Leaf
        if len(mesh.children) == 0:            
            sceneFile.write(numTabs*'\t'+'<Mesh name="'+mesh.name+'" material="'+mat+'" ')
            if s != Vector(1,1,1):
                sceneFile.write('sx="%g" sy="%g" sz="%g" ' % tuple(s))
            if t != Vector():
                sceneFile.write('tx="%g" ty="%g" tz="%g" ' % tuple(t))
            if r.x != 0 or r.y != 0 or r.z != 0:
                sceneFile.write('rx="%g" ry="%g" rz="%g" ' % tuple(r))
            sceneFile.write('batchStart="'+str(mesh.batchStart)+'" batchCount="'+str(mesh.batchCount)+'" ')
            sceneFile.write('vertRStart="'+str(mesh.triGroups[0].vertRStart)+'" vertREnd="'+str(mesh.triGroups[-1].vertREnd)+'" />\n')
        # Node        
        else:
            sceneFile.write(numTabs*'\t'+'<Mesh name="'+mesh.name+'" material="'+mat+'" ')
            if s != Vector(1,1,1):
                sceneFile.write('sx="%g" sy="%g" sz="%g" ' % tuple(s))
            if t != Vector():
                sceneFile.write('tx="%g" ty="%g" tz="%g" ' % tuple(t))
            if r.x != 0 or r.y != 0 or r.z != 0:
                sceneFile.write('rx="%g" ry="%g" rz="%g" ' % tuple(r))
            sceneFile.write('batchStart="'+str(mesh.batchStart)+'" batchCount="'+str(mesh.batchCount)+'" ')
            sceneFile.write('vertRStart="'+str(mesh.triGroups[0].vertRStart)+'" vertREnd="'+str(mesh.triGroups[-1].vertREnd)+'" >\n')
            for child in  mesh.children:
                self.writeMesh(sceneFile, child, fileName, numTabs+1)
            sceneFile.write(numTabs*'\t'+'</Mesh>\n')
    
    def writeJoint( self, sceneFile, joint, numTabs):
        s = roundSize(joint.matRel.scalePart())
        r = roundVec(joint.matRel.toEuler())
        t = roundVec(joint.matRel.translationPart())
        
        # Leaf
        if len(joint.children) == 0:            
            sceneFile.write(numTabs*'\t'+'<Joint name="'+joint.name+'" ')
            if s != Vector(1,1,1):
                sceneFile.write('sx="%g" sy="%g" sz="%g" ' % tuple(s))
            if t != Vector():
                sceneFile.write('tx="%g" ty="%g" tz="%g" ' % tuple(t))
            if r.x != 0 or r.y != 0 or r.z != 0:
                sceneFile.write('rx="%g" ry="%g" rz="%g" ' % tuple(r))
            sceneFile.write('jointIndex="'+str(joint.index)+'" />\n')
        # Node        
        else:
            sceneFile.write(numTabs*'\t'+'<Joint name="'+joint.name+'" ')
            if s != Vector(1,1,1):
                sceneFile.write('sx="%g" sy="%g" sz="%g" ' % tuple(s))
            if t != Vector():
                sceneFile.write('tx="%g" ty="%g" tz="%g" ' % tuple(t))
            if r.x != 0 or r.y != 0 or r.z != 0:
                sceneFile.write('rx="%g" ry="%g" rz="%g" ' % tuple(r))
            sceneFile.write('jointIndex="'+str(joint.index)+'" >\n')
            for child in  joint.children:
                self.writeJoint(sceneFile, child, numTabs+1)
            sceneFile.write(numTabs*'\t'+'</Joint>\n')
        
    def writeMaterials( self, fileName, defShader ):
        try:
            if not Blender.sys.exists(self.__matPath+fileName):
                os.makedirs(self.__matPath+fileName)
            os.chdir(self.__matPath+fileName)
        except:
            print "Can't create directory: "+self.__matPath+fileName
            return False
            
        # Write materials
        for mat in self.__materials:
            try:
                matFile = open(self.__materials[mat]['name']+".material.xml", "w" )
            except:
                print "Error creating file: %s.material.xml" % mat
                return False
            matFile.write('<Material>\n')
            matFile.write('\t<Shader source="%s" />\n\n' % defShader)
            for tex in self.__materials[mat]['textures']:
                matFile.write('\t<TexUnit unit="%i" map="%s" />\n' % tex)
            if self.__materials[mat]['uniform'] != None:
                color = tuple(self.__materials[mat]['uniform'])
                name = [self.__materials[mat]['name']+'col']
                matFile.write('\t<Uniform name="%s" a="%s" b="%s" c="%s" d="%s" />\n' % (tuple(name)+color))
            matFile.write('</Material>\n')    
            matFile.close()
            
        os.chdir(Blender.sys.dirname(self.__filePath))
        return True
        
    def writeAnimation( self, fileName ):
        try:
            if not Blender.sys.exists(self.__animPath):
                os.makedirs(self.__animPath)
            os.chdir(self.__animPath)
        except:
            print "Can't change directory: "+self.__animPath
            return False
        try:
            animFile = open(fileName+".anim", "wb")
        except:
            print "Error creating file: %s.anim" % fileName
            return False
        
        # Write anim header
        animFile.write('H3DA')
        version = struct.pack('i', 2)
        animFile.write(version)
        
        # Write number of animated joints and meshes , number of frames
        animFile.write( struct.pack('i', self.__animCount) )
        animFile.write( struct.pack('i', self.__frameCount) )

        # write Animation data
        for joint in self.__joints:
            if len(joint.frames) > 0:
                animFile.write(str256(joint.name))
                
                for frame in joint.frames:
                    s = roundSize(frame.scalePart())
                    t = roundVec(frame.translationPart())
                    r = roundQuat(frame.toQuat())
                    animFile.write( struct.pack('f', r.x))
                    animFile.write( struct.pack('f', r.y))
                    animFile.write( struct.pack('f', r.z))
                    animFile.write( struct.pack('f', r.w))
                    animFile.write( struct.pack('f', t.x))
                    animFile.write( struct.pack('f', t.y))
                    animFile.write( struct.pack('f', t.z))
                    animFile.write( struct.pack('f', s.x))
                    animFile.write( struct.pack('f', s.y))
                    animFile.write( struct.pack('f', s.z))
            
        for mesh in self.__meshes:
            if len(mesh.frames) > 0:
                animFile.write(str256(mesh.name))
                
                for frame in mesh.frames:
                    s = roundSize(frame.scalePart())
                    t = roundVec(frame.translationPart())
                    r = roundQuat(frame.toQuat())
                    animFile.write( struct.pack('f', r.x))
                    animFile.write( struct.pack('f', r.y))
                    animFile.write( struct.pack('f', r.z))
                    animFile.write( struct.pack('f', r.w))
                    animFile.write( struct.pack('f', t.x))
                    animFile.write( struct.pack('f', t.y))
                    animFile.write( struct.pack('f', t.z))
                    animFile.write( struct.pack('f', s.x))
                    animFile.write( struct.pack('f', s.y))
                    animFile.write( struct.pack('f', s.z))
        
        # Close anim file
        animFile.close()    
        
        os.chdir(Blender.sys.dirname(self.__filePath))
        return True
    
        
    def calcTangentSpaceBasis( self, mesh, bmesh, dicG2L, dicL2Dat ):        
        # based on Algorithm: Eric Lengyel, Mathematics for 3D Game Programming & Computer Graphics
        
        for tri in mesh.triGroups:
            k = tri.first
            while k < tri.first + tri.count:
                dat0 = dicL2Dat[dicG2L[self.__indices[k]]]
                dat1 = dicL2Dat[dicG2L[self.__indices[k+1]]]
                dat2 = dicL2Dat[dicG2L[self.__indices[k+2]]]
                
                v0 = bmesh.verts[dat0.bIdx].co
                v1 = bmesh.verts[dat1.bIdx].co
                v2 = bmesh.verts[dat2.bIdx].co
                
                v0_str = dat0.uvs
                v1_str = dat1.uvs
                v2_str = dat2.uvs

                xyz0 = v1 - v0
                xyz1 = v2 - v0
                st0 = [v1_str[0] - v0_str[0], v1_str[1] - v0_str[1]]
                st1 = [v2_str[0] - v0_str[0], v2_str[1] - v0_str[1]]

                r = 0.0
                cp = st0[0] * st1[1] - st1[0] * st0[1]
                if  cp != 0:
                        r = 1.0 / cp
                sdir = Vector( (st1[1] * xyz0.x - st0[1] * xyz1.x) * r, (st1[1] * xyz0.y - st0[1] * xyz1.y) * r, (st1[1] * xyz0.z - st0[1] * xyz1.z) * r )
                tdir = Vector( (st0[0] * xyz1.x - st1[0] * xyz0.x) * r, (st0[0] * xyz1.y - st1[0] * xyz0.y) * r, (st0[0] * xyz1.z - st1[0] * xyz0.z) * r )
                              
                dat0.nors += dat0.bNors
                dat1.nors += dat1.bNors
                dat2.nors += dat2.bNors

                dat0.tans += sdir
                dat1.tans += sdir
                dat2.tans += sdir
                 
                dat0.bitans += tdir
                dat1.bitans += tdir
                dat2.bitans += tdir  
                  
                for l in range(3):
                    vi = self.__indices[k + l]
                    datVi = dicL2Dat[dicG2L[vi]]
                    indices = tri.posIndexToVertices[self.__vertices[vi].posIndex]
                    for i in indices[1:]:
                        if vi == i:
                            continue
                        
                        datI = dicL2Dat[dicG2L[i]]
                        if datI.bNors == datVi.bNors:
                            datI.nors   += datI.bNors
                            datI.tans   += sdir
                            datI.bitans += tdir
                                
                k += 3

        # Normalize tangent space basis
        for dat in dicL2Dat:
            n = dat.nors.normalize()
            t = dat.tans
            dat.tans = (t - n * (n * t)).normalize()
                        
            if CrossVecs( n, t ) * dat.bitans < 0.0:
                dat.bitans = CrossVecs( (n * -1), t ).normalize()
            else:
                dat.bitans = CrossVecs( n, t ).normalize()
                
    def copyTextures(self):
        try:
            if not Blender.sys.exists(self.__texPath):
                os.makedirs(self.__texPath)
            os.chdir(self.__texPath)
        except:
            print "Can't change directory: "+self.__texPath
            return False

        error = False
        for image in self.__images:
            name = Blender.sys.basename(image.filename)
            oldFile = image.getFilename()
            
            if Blender.sys.exists(self.__texPath+name):
                if not Blender.Draw.PupMenu( "Image "+name+" Already Exists, Overwrite?%t|Yes%x1|No%x0"):
                    print "Copying image %s aborted by User!" % name
                    continue
    
            image.setFilename(self.__texPath+name)
            
            try:
                image.save()
                image.setFilename(oldFile)
            except:
                image.setFilename(oldFile)
                error = True
                print 'Error while copying image: '+image.getFilename()
                    
        os.chdir(Blender.sys.dirname(self.__filePath))
        
        return not error
        
def roundVec(v):
    return Vector(round(v.x, 5), round(v.z, 5), -round(v.y, 5))

def roundSize(v):
    return Vector(round(v.x, 5), round(v.z, 5), round(v.y, 5))

def roundEuler(e):
    return Euler(round(e.x, 5), round(e.z, 5), -round(e.y, 5))

def roundQuat(q):
    return Quaternion(round(q.w, 7), round(q.x, 7), round(q.z, 7), -round(q.y, 7))

def hordeMatrix(m):
    row1 =[ m[0][0], m[0][2], -m[0][1], m[0][3]]
    row2 =[ m[2][0], m[2][2], -m[2][1], m[2][3]]
    row3 =[ -m[1][0], -m[1][2], m[1][1], -m[1][3]]
    row4 =[ m[3][0], m[3][2], -m[3][1], m[3][3]]    
    return Matrix(row1, row2, row3, row4)

def str256 (s):
    if len(s) > 255:
        return s[:255]+chr(0)
    else:
        fill = 256-len(s)
        return s+fill*chr(0)