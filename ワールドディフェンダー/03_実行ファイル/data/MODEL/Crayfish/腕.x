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
 26;
 -4.80650;534.87482;-2417.30884;,
 -4.80720;5.86450;0.24610;,
 264.33881;379.92960;-2417.31030;,
 -4.80720;5.86450;0.24610;,
 375.82349;5.86450;-2417.30884;,
 -4.80720;5.86450;0.24610;,
 264.33881;-368.20099;-2417.31030;,
 -4.80720;5.86450;0.24610;,
 -4.80670;-523.14673;-2417.30933;,
 -4.80720;5.86450;0.24610;,
 -273.95441;-368.20099;-2417.30835;,
 -4.80720;5.86450;0.24610;,
 -385.43939;5.86450;-2417.31030;,
 -4.80720;5.86450;0.24610;,
 -273.95441;379.92960;-2417.30835;,
 -4.80720;5.86450;0.24610;,
 -4.80650;534.87482;-2417.30884;,
 -4.80680;5.86450;-3576.60278;,
 -4.80650;534.87482;-2417.30884;,
 264.33881;379.92960;-2417.31030;,
 375.82349;5.86450;-2417.30884;,
 264.33881;-368.20099;-2417.31030;,
 -4.80670;-523.14673;-2417.30933;,
 -273.95441;-368.20099;-2417.30835;,
 -385.43939;5.86450;-2417.31030;,
 -273.95441;379.92960;-2417.30835;;
 
 16;
 3;0,1,2;,
 3;2,3,4;,
 3;4,5,6;,
 3;6,7,8;,
 3;8,9,10;,
 3;10,11,12;,
 3;12,13,14;,
 3;14,15,16;,
 3;17,18,19;,
 3;17,19,20;,
 3;17,20,21;,
 3;17,21,22;,
 3;17,22,23;,
 3;17,23,24;,
 3;17,24,25;,
 3;17,25,18;;
 
 MeshMaterialList {
  2;
  16;
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
   0.800000;0.053600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  14;
  0.000002;0.994061;-0.108821;,
  0.782986;0.615135;-0.092422;,
  0.996729;0.000000;-0.080812;,
  0.782987;-0.615134;-0.092422;,
  0.000001;-0.994061;-0.108822;,
  -0.782985;-0.615136;-0.092422;,
  -0.996729;0.000000;-0.080813;,
  -0.782985;0.615137;-0.092422;,
  0.925014;0.330182;0.187961;,
  0.876400;0.308124;-0.370111;,
  0.000002;-0.976886;0.213763;,
  -0.925013;0.330183;0.187962;,
  0.000001;-0.909757;-0.415142;,
  -0.876399;0.308124;-0.370113;;
  16;
  3;0,8,1;,
  3;1,8,2;,
  3;2,8,3;,
  3;3,10,4;,
  3;4,10,5;,
  3;5,11,6;,
  3;6,11,7;,
  3;7,11,0;,
  3;9,0,1;,
  3;9,1,2;,
  3;9,2,3;,
  3;12,3,4;,
  3;12,4,5;,
  3;13,5,6;,
  3;13,6,7;,
  3;13,7,0;;
 }
 MeshTextureCoords {
  26;
  0.000000;1.000000;,
  0.062500;0.000000;,
  0.125000;1.000000;,
  0.125000;0.000000;,
  0.250000;1.000000;,
  0.187500;0.000000;,
  0.375000;1.000000;,
  0.250000;0.000000;,
  0.500000;1.000000;,
  0.312500;0.000000;,
  0.625000;1.000000;,
  0.375000;0.000000;,
  0.750000;1.000000;,
  0.437500;0.000000;,
  0.875000;1.000000;,
  0.500000;0.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
