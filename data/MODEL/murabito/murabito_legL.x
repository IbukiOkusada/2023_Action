xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 18;
 1.74862;-5.93331;0.92701;,
 1.74862;-0.12006;0.92701;,
 0.53844;-0.12006;1.42828;,
 0.53844;-5.93331;1.42828;,
 -0.67173;-0.12006;0.92701;,
 -0.67173;-5.93331;0.92701;,
 -1.17300;-0.12006;-0.28317;,
 -1.17300;-5.93331;-0.28317;,
 -0.67173;-0.12006;-1.49334;,
 -0.67173;-5.93331;-1.49334;,
 0.53844;-0.12006;-1.99462;,
 0.53844;-5.93331;-1.99462;,
 1.74862;-0.12006;-1.49334;,
 1.74862;-5.93331;-1.49334;,
 2.24989;-0.12006;-0.28317;,
 2.24989;-5.93331;-0.28317;,
 0.53844;-5.93331;-0.28317;,
 0.53844;-0.12006;-0.28317;;
 
 24;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,1,0;,
 3;3,16,0;,
 3;5,16,3;,
 3;7,16,5;,
 3;9,16,7;,
 3;11,16,9;,
 3;13,16,11;,
 3;15,16,13;,
 3;0,16,15;,
 3;1,17,2;,
 3;2,17,4;,
 3;4,17,6;,
 3;6,17,8;,
 3;8,17,10;,
 3;10,17,12;,
 3;12,17,14;,
 3;14,17,1;;
 
 MeshMaterialList {
  1;
  24;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\murabito002.png";
   }
  }
 }
 MeshNormals {
  10;
  0.707107;0.000000;0.707107;,
  -0.000001;0.000000;1.000000;,
  -0.707108;0.000000;0.707106;,
  -1.000000;0.000000;-0.000001;,
  -0.707109;0.000000;-0.707104;,
  -0.000001;0.000000;-1.000000;,
  0.707108;0.000000;-0.707105;,
  1.000000;0.000000;-0.000001;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;;
  24;
  4;0,0,1,1;,
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;6,6,7,7;,
  4;7,7,0,0;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;8,8,8;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;;
 }
 MeshTextureCoords {
  18;
  0.756060;0.486040;,
  0.756060;0.295830;,
  0.772090;0.295830;,
  0.772090;0.486040;,
  0.788110;0.295830;,
  0.788110;0.486040;,
  0.794750;0.295830;,
  0.794750;0.486040;,
  0.788110;0.295830;,
  0.788110;0.486040;,
  0.772090;0.295830;,
  0.772090;0.486040;,
  0.756060;0.295830;,
  0.756060;0.486040;,
  0.749420;0.295830;,
  0.749420;0.486040;,
  0.772090;0.486040;,
  0.772090;0.295830;;
 }
}
