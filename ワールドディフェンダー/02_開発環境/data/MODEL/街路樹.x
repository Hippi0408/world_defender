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
 110;
 0.00000;440.96773;0.00000;,
 20.96916;440.96773;-20.96916;,
 0.00000;440.96773;-29.65488;,
 0.00000;440.96773;0.00000;,
 29.65488;440.96773;0.00000;,
 0.00000;440.96773;0.00000;,
 20.96916;440.96773;20.96916;,
 0.00000;440.96773;0.00000;,
 0.00000;440.96773;29.65488;,
 0.00000;440.96773;0.00000;,
 -20.96916;440.96773;20.96916;,
 0.00000;440.96773;0.00000;,
 -29.65488;440.96773;0.00000;,
 0.00000;440.96773;0.00000;,
 -20.96916;440.96773;-20.96916;,
 0.00000;440.96773;0.00000;,
 0.00000;440.96773;-29.65488;,
 28.28428;7.62760;-28.28428;,
 31.83680;-29.74952;-97.09252;,
 31.83680;-11.12948;-97.09252;,
 28.28428;26.24768;-28.28428;,
 0.00000;7.62760;0.00000;,
 110.43372;-27.96364;89.68300;,
 110.43372;-9.34360;89.68300;,
 0.00000;7.62760;0.00000;,
 28.28428;7.62760;28.28428;,
 28.28428;26.24768;28.28428;,
 0.00000;7.62760;0.00000;,
 -61.47980;-35.36028;107.76796;,
 -61.47980;-16.74024;107.76796;,
 0.00000;7.62760;0.00000;,
 -28.28428;7.62760;28.28428;,
 -28.28428;26.24768;28.28428;,
 0.00000;7.62760;0.00000;,
 -128.33240;-25.90556;-25.28336;,
 -128.33240;-7.28552;-25.28336;,
 0.00000;7.62760;0.00000;,
 -28.28428;7.62760;-28.28428;,
 -28.28428;26.24768;-28.28428;,
 0.00000;7.62760;0.00000;,
 31.83680;-29.74952;-97.09252;,
 31.83680;-11.12948;-97.09252;,
 0.00000;7.62760;0.00000;,
 0.00000;52.01532;-39.35724;,
 27.82976;52.01532;-27.82976;,
 0.00000;52.01532;-39.35724;,
 -27.82976;52.01532;-27.82976;,
 39.35724;52.01532;0.00000;,
 -39.35724;52.01532;0.00000;,
 27.82976;52.01532;27.82976;,
 -27.82976;52.01532;27.82976;,
 0.00000;52.01532;39.35724;,
 0.00000;656.56964;-136.08504;,
 0.00000;724.28190;0.00000;,
 96.22668;656.56964;-96.22668;,
 0.00000;724.28190;0.00000;,
 136.08504;656.56964;0.00000;,
 0.00000;724.28190;0.00000;,
 96.22668;656.56964;96.22668;,
 0.00000;724.28190;0.00000;,
 0.00000;656.56964;136.08504;,
 0.00000;724.28190;0.00000;,
 -96.22668;656.56964;96.22664;,
 0.00000;724.28190;0.00000;,
 -136.08504;656.56964;0.00000;,
 0.00000;724.28190;0.00000;,
 -96.22668;656.56964;-96.22664;,
 0.00000;724.28190;0.00000;,
 0.00000;656.56964;-136.08504;,
 0.00000;279.57752;0.00000;,
 0.00000;279.57752;-113.71880;,
 80.41132;279.57752;-80.41132;,
 113.71880;279.57752;0.00000;,
 80.41132;279.57752;80.41132;,
 0.00000;279.57752;113.71880;,
 -80.41132;279.57752;80.41128;,
 -113.71880;279.57752;0.00000;,
 -80.41132;279.57752;-80.41128;,
 119.24376;590.13761;-119.24376;,
 96.22668;656.56964;-96.22668;,
 136.08504;656.56964;0.00000;,
 168.63604;590.13761;0.00000;,
 0.00000;590.13761;-168.63604;,
 0.00000;656.56964;-136.08504;,
 96.22668;656.56964;96.22668;,
 119.24376;590.13761;119.24376;,
 -119.24376;590.13761;-119.24376;,
 -96.22668;656.56964;-96.22664;,
 0.00000;656.56964;136.08504;,
 0.00000;590.13761;168.63604;,
 -168.63604;590.13761;0.00000;,
 -136.08504;656.56964;0.00000;,
 -96.22668;656.56964;96.22664;,
 -119.24376;590.13761;119.24376;,
 0.00000;511.33038;-153.18056;,
 108.31504;511.33038;-108.31504;,
 -108.31504;511.33038;-108.31504;,
 153.18056;511.33038;0.00000;,
 -153.18056;511.33038;0.00000;,
 108.31504;511.33038;108.31504;,
 -108.31504;511.33038;108.31504;,
 0.00000;511.33038;153.18056;,
 126.10805;428.82437;-126.10805;,
 178.34364;428.82437;0.00000;,
 0.00000;428.82437;-178.34364;,
 126.10805;428.82437;126.10805;,
 -126.10805;428.82437;-126.10800;,
 0.00000;428.82437;178.34364;,
 -178.34364;428.82437;0.00000;,
 -126.10805;428.82437;126.10800;;
 
 88;
 3;0,1,2;,
 3;3,4,1;,
 3;5,6,4;,
 3;7,8,6;,
 3;9,10,8;,
 3;11,12,10;,
 3;13,14,12;,
 3;15,16,14;,
 4;17,18,19,20;,
 3;21,18,17;,
 4;22,17,20,23;,
 3;24,17,22;,
 4;25,22,23,26;,
 3;27,22,25;,
 4;28,25,26,29;,
 3;30,25,28;,
 4;31,28,29,32;,
 3;33,28,31;,
 4;34,31,32,35;,
 3;36,31,34;,
 4;37,34,35,38;,
 3;39,34,37;,
 4;40,37,38,41;,
 3;42,37,40;,
 4;43,2,1,44;,
 4;43,44,20,19;,
 4;45,41,38,46;,
 4;45,46,14,16;,
 4;44,1,4,47;,
 4;44,47,23,20;,
 4;46,38,35,48;,
 4;46,48,12,14;,
 4;47,4,6,49;,
 4;47,49,26,23;,
 4;48,35,32,50;,
 4;48,50,10,12;,
 4;49,6,8,51;,
 4;49,51,29,26;,
 4;50,32,29,51;,
 4;50,51,8,10;,
 3;52,53,54;,
 3;54,55,56;,
 3;56,57,58;,
 3;58,59,60;,
 3;60,61,62;,
 3;62,63,64;,
 3;64,65,66;,
 3;66,67,68;,
 3;69,70,71;,
 3;69,71,72;,
 3;69,72,73;,
 3;69,73,74;,
 3;69,74,75;,
 3;69,75,76;,
 3;69,76,77;,
 3;69,77,70;,
 4;78,79,80,81;,
 4;78,82,83,79;,
 4;81,80,84,85;,
 4;82,86,87,83;,
 4;85,84,88,89;,
 4;86,90,91,87;,
 4;89,88,92,93;,
 4;90,93,92,91;,
 4;94,82,78,95;,
 4;94,96,86,82;,
 4;95,78,81,97;,
 4;96,98,90,86;,
 4;97,81,85,99;,
 4;98,100,93,90;,
 4;99,85,89,101;,
 4;100,101,89,93;,
 4;102,95,97,103;,
 4;102,103,72,71;,
 4;102,71,70,104;,
 4;102,104,94,95;,
 4;103,97,99,105;,
 4;103,105,73,72;,
 4;104,70,77,106;,
 4;104,106,96,94;,
 4;105,99,101,107;,
 4;105,107,74,73;,
 4;106,77,76,108;,
 4;106,108,98,96;,
 4;107,101,100,109;,
 4;107,109,75,74;,
 4;108,76,75,109;,
 4;108,109,100,98;;
 
 MeshMaterialList {
  2;
  88;
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
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.592800;0.213600;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.501600;0.019200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  96;
  0.000000;1.000000;0.000000;,
  0.907663;0.210342;-0.363186;,
  0.885575;0.130938;-0.445659;,
  0.855745;0.058667;-0.514060;,
  -0.598668;0.000000;0.800997;,
  0.575470;0.076989;0.814191;,
  -0.922758;0.000000;-0.385380;,
  -0.646493;0.147533;0.748519;,
  -0.524806;0.157862;-0.836455;,
  0.998670;0.000000;0.051561;,
  0.961509;0.000000;-0.274772;,
  0.820627;0.000000;-0.571464;,
  -0.639945;-0.194755;0.743331;,
  0.662934;0.000000;0.748678;,
  -0.797160;-0.325114;-0.508760;,
  -0.472019;0.000000;0.881588;,
  -0.427143;0.000000;-0.904184;,
  0.135137;-0.990066;0.038821;,
  0.231276;0.158323;-0.959919;,
  0.744578;0.142481;-0.652153;,
  -0.657636;0.168342;-0.734286;,
  0.984845;0.058754;-0.163182;,
  -0.956748;0.168288;-0.237303;,
  0.616749;0.192819;0.763179;,
  -0.723477;0.205912;0.658924;,
  0.173005;0.071952;0.982289;,
  0.095376;-0.918097;0.384708;,
  0.152385;-0.976503;0.152385;,
  -0.172480;-0.969794;-0.172480;,
  -0.653731;-0.381144;0.653731;,
  0.570502;0.231667;0.787946;,
  0.239040;-0.941127;-0.239040;,
  -0.182585;-0.872939;-0.452371;,
  -0.562594;-0.605786;-0.562594;,
  -0.654925;0.252501;0.712261;,
  0.208579;-0.955505;0.208579;,
  0.260849;-0.964329;-0.045026;,
  -0.029981;0.000000;-0.999550;,
  -0.387140;0.218933;-0.895651;,
  0.014925;-0.999777;-0.014925;,
  -0.753044;0.000000;-0.657971;,
  -0.639593;0.097877;-0.762457;,
  0.000000;0.024937;-0.999689;,
  0.706887;0.024937;-0.706887;,
  -0.504799;0.192488;-0.841502;,
  -0.706887;0.024937;-0.706887;,
  0.999689;0.024937;0.000000;,
  -0.999689;0.024937;0.000000;,
  0.706887;0.024937;0.706887;,
  0.518586;0.521175;0.677823;,
  -0.775767;0.284751;0.563118;,
  -0.706887;0.024937;0.706887;,
  0.000000;0.024937;0.999689;,
  -0.648099;0.446191;0.617156;,
  -0.000000;0.710698;-0.703497;,
  0.497447;0.710698;-0.497447;,
  0.703497;0.710698;0.000000;,
  0.497447;0.710698;0.497447;,
  -0.000000;0.710698;0.703497;,
  -0.497447;0.710698;0.497447;,
  -0.703497;0.710698;0.000000;,
  -0.497447;0.710698;-0.497447;,
  -0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  0.700868;0.132545;-0.700868;,
  0.991177;0.132545;0.000000;,
  -0.000000;0.132545;-0.991177;,
  0.700868;0.132545;0.700868;,
  -0.700868;0.132545;-0.700868;,
  -0.000000;0.132545;0.991177;,
  -0.991177;0.132545;0.000000;,
  -0.700868;0.132545;0.700868;,
  -0.000000;0.051603;-0.998668;,
  0.706165;0.051603;-0.706165;,
  -0.706165;0.051603;-0.706165;,
  0.998668;0.051603;0.000000;,
  -0.998668;0.051603;0.000000;,
  0.706165;0.051603;0.706165;,
  -0.706165;0.051603;0.706165;,
  -0.000000;0.051603;0.998668;,
  0.705946;-0.057271;-0.705946;,
  0.998359;-0.057271;-0.000000;,
  -0.000000;-0.057271;-0.998359;,
  0.705946;-0.057271;0.705946;,
  -0.705946;-0.057271;-0.705946;,
  -0.000000;-0.057271;0.998359;,
  -0.998359;-0.057271;0.000000;,
  -0.705946;-0.057271;0.705946;,
  0.917665;-0.397355;-0.000000;,
  0.648887;-0.397355;-0.648887;,
  -0.000000;-0.397355;-0.917665;,
  0.648887;-0.397355;0.648887;,
  -0.648887;-0.397355;-0.648887;,
  -0.000000;-0.397355;0.917665;,
  -0.917665;-0.397355;0.000000;,
  -0.648887;-0.397355;0.648887;;
  88;
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  4;10,9,1,2;,
  3;17,26,27;,
  4;11,10,2,3;,
  3;17,27,28;,
  4;12,12,4,4;,
  3;29,12,12;,
  4;13,13,30,5;,
  3;17,31,32;,
  4;14,6,6,6;,
  3;33,32,14;,
  4;15,15,34,7;,
  3;17,35,36;,
  4;16,37,38,8;,
  3;17,36,39;,
  4;40,16,8,41;,
  3;17,39,26;,
  4;18,42,43,19;,
  4;18,19,2,1;,
  4;44,41,8,20;,
  4;18,20,45,42;,
  4;19,43,46,21;,
  4;19,21,3,2;,
  4;20,8,38,22;,
  4;20,22,47,45;,
  4;21,46,48,23;,
  4;49,23,30,49;,
  4;50,7,34,24;,
  4;22,24,51,47;,
  4;23,48,52,25;,
  4;23,25,5,30;,
  4;24,34,53,53;,
  4;24,25,52,51;,
  3;54,62,55;,
  3;55,62,56;,
  3;56,62,57;,
  3;57,62,58;,
  3;58,62,59;,
  3;59,62,60;,
  3;60,62,61;,
  3;61,62,54;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  3;63,63,63;,
  4;64,55,56,65;,
  4;64,66,54,55;,
  4;65,56,57,67;,
  4;66,68,61,54;,
  4;67,57,58,69;,
  4;68,70,60,61;,
  4;69,58,59,71;,
  4;70,71,59,60;,
  4;72,66,64,73;,
  4;72,74,68,66;,
  4;73,64,65,75;,
  4;74,76,70,68;,
  4;75,65,67,77;,
  4;76,78,71,70;,
  4;77,67,69,79;,
  4;78,79,69,71;,
  4;80,73,75,81;,
  4;80,81,88,89;,
  4;80,89,90,82;,
  4;80,82,72,73;,
  4;81,75,77,83;,
  4;81,83,91,88;,
  4;82,90,92,84;,
  4;82,84,74,72;,
  4;83,77,79,85;,
  4;83,85,93,91;,
  4;84,92,94,86;,
  4;84,86,76,74;,
  4;85,79,78,87;,
  4;85,87,95,93;,
  4;86,94,95,87;,
  4;86,87,78,76;;
 }
 MeshTextureCoords {
  110;
  0.062500;0.000000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.187500;0.000000;,
  0.250000;0.000000;,
  0.312500;0.000000;,
  0.375000;0.000000;,
  0.437500;0.000000;,
  0.500000;0.000000;,
  0.562500;0.000000;,
  0.625000;0.000000;,
  0.687500;0.000000;,
  0.750000;0.000000;,
  0.812500;0.000000;,
  0.875000;0.000000;,
  0.937500;0.000000;,
  1.000000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.062500;1.000000;,
  0.250000;1.000000;,
  0.250000;1.000000;,
  0.187500;1.000000;,
  0.375000;1.000000;,
  0.375000;1.000000;,
  0.312500;1.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  0.437500;1.000000;,
  0.625000;1.000000;,
  0.625000;1.000000;,
  0.562500;1.000000;,
  0.750000;1.000000;,
  0.750000;1.000000;,
  0.687500;1.000000;,
  0.875000;1.000000;,
  0.875000;1.000000;,
  0.812500;1.000000;,
  1.000000;1.000000;,
  1.000000;1.000000;,
  0.937500;1.000000;,
  0.000000;0.937870;,
  0.125000;0.937870;,
  1.000000;0.937870;,
  0.875000;0.937870;,
  0.250000;0.937870;,
  0.750000;0.937870;,
  0.375000;0.937870;,
  0.625000;0.937870;,
  0.500000;0.937870;,
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
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
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
