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
 -0.99525;-5.88479;0.92701;,
 0.21492;-5.88479;1.42828;,
 0.21492;-0.07155;1.42828;,
 -0.99525;-0.07155;0.92701;,
 1.42510;-5.88479;0.92701;,
 1.42510;-0.07155;0.92701;,
 1.92637;-5.88479;-0.28317;,
 1.92637;-0.07155;-0.28317;,
 1.42510;-5.88479;-1.49334;,
 1.42510;-0.07155;-1.49334;,
 0.21492;-5.88479;-1.99462;,
 0.21492;-0.07155;-1.99462;,
 -0.99525;-5.88479;-1.49334;,
 -0.99525;-0.07155;-1.49334;,
 -1.49653;-5.88479;-0.28317;,
 -1.49653;-0.07155;-0.28317;,
 0.21492;-5.88479;-0.28317;,
 0.21492;-0.07155;-0.28317;;
 
 24;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;1,0,16;,
 3;4,1,16;,
 3;6,4,16;,
 3;8,6,16;,
 3;10,8,16;,
 3;12,10,16;,
 3;14,12,16;,
 3;0,14,16;,
 3;3,2,17;,
 3;2,5,17;,
 3;5,7,17;,
 3;7,9,17;,
 3;9,11,17;,
 3;11,13,17;,
 3;13,15,17;,
 3;15,3,17;;
 
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
  -0.707105;0.000000;0.707108;,
  -0.000001;0.000000;1.000000;,
  0.707107;0.000000;0.707107;,
  1.000000;0.000000;-0.000001;,
  0.707108;0.000000;-0.707105;,
  -0.000001;0.000000;-1.000000;,
  -0.707107;0.000000;-0.707107;,
  -1.000000;0.000000;-0.000001;,
  0.000000;-1.000000;-0.000000;,
  0.000000;1.000000;0.000000;;
  24;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,0,0,7;,
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
  0.704120;0.486040;,
  0.688090;0.486040;,
  0.688090;0.295830;,
  0.704120;0.295830;,
  0.672060;0.486040;,
  0.672060;0.295830;,
  0.665430;0.486040;,
  0.665430;0.295830;,
  0.672060;0.486040;,
  0.672060;0.295830;,
  0.688090;0.486040;,
  0.688090;0.295830;,
  0.704120;0.486040;,
  0.704120;0.295830;,
  0.710760;0.486040;,
  0.710760;0.295830;,
  0.688090;0.486040;,
  0.688090;0.295830;;
 }
}
