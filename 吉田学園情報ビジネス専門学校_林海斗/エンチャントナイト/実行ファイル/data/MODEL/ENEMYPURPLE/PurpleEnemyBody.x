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
 84;
 -38.63691;-10.93000;0.00000;,
 -27.32039;-10.93000;-27.32039;,
 -23.87834;-29.81938;-23.87834;,
 -33.76900;-29.81938;0.00000;,
 -29.04303;-0.10582;-29.04303;,
 -41.07311;-0.10582;0.00000;,
 -27.01525;-29.81938;0.00000;,
 -19.10264;-29.81938;-19.10264;,
 0.00000;-10.93000;-38.63691;,
 0.00000;-29.81938;-33.76900;,
 0.00000;-0.10582;-41.07311;,
 0.00000;-29.81938;-27.01525;,
 27.32039;-10.93000;-27.32039;,
 23.87834;-29.81938;-23.87834;,
 29.04303;-0.10582;-29.04303;,
 19.10264;-29.81938;-19.10264;,
 38.63691;-10.93000;-0.00000;,
 33.76900;-29.81938;-0.00000;,
 41.07311;-0.10582;-0.00000;,
 27.01525;-29.81938;-0.00000;,
 27.32039;-10.93000;27.32047;,
 23.87834;-29.81938;23.87834;,
 29.04298;-0.10582;29.04303;,
 19.10264;-29.81938;19.10264;,
 0.00000;-10.93000;38.63691;,
 0.00000;-29.81938;33.76900;,
 0.00000;-0.10582;41.07311;,
 0.00000;-29.81938;27.01525;,
 -27.32039;-10.93000;27.32047;,
 -23.87834;-29.81938;23.87834;,
 -29.04303;-0.10582;29.04303;,
 -19.10264;-29.81938;19.10264;,
 -38.63691;-10.93000;0.00000;,
 -33.76900;-29.81938;0.00000;,
 -41.07311;-0.10582;0.00000;,
 -27.01525;-29.81938;0.00000;,
 -37.40671;-88.20380;-37.40671;,
 -52.90103;-88.20380;0.00000;,
 -42.32090;-88.20380;0.00000;,
 -29.92537;-88.20380;-29.92537;,
 0.00000;-88.20380;-52.90103;,
 0.00000;-88.20380;-42.32090;,
 37.40671;-88.20380;-37.40671;,
 29.92537;-88.20380;-29.92537;,
 52.90103;-88.20380;-0.00000;,
 42.32090;-88.20380;-0.00000;,
 37.40671;-88.20380;37.40671;,
 29.92537;-88.20380;29.92537;,
 0.00000;-88.20380;52.90103;,
 0.00000;-88.20380;42.32090;,
 -37.40671;-88.20380;37.40671;,
 -29.92537;-88.20380;29.92537;,
 -52.90103;-88.20380;0.00000;,
 -42.32090;-88.20380;0.00000;,
 -29.04303;-0.10582;-29.04303;,
 -27.32039;-10.93000;-27.32039;,
 -52.90103;-88.20380;0.00000;,
 -37.40671;-88.20380;-37.40671;,
 -29.92537;-88.20380;-29.92537;,
 -42.32090;-88.20380;0.00000;,
 0.00000;-0.10582;-41.07311;,
 0.00000;-10.93000;-38.63691;,
 0.00000;-88.20380;-52.90103;,
 0.00000;-88.20380;-42.32090;,
 29.04303;-0.10582;-29.04303;,
 27.32039;-10.93000;-27.32039;,
 37.40671;-88.20380;-37.40671;,
 29.92537;-88.20380;-29.92537;,
 41.07311;-0.10582;-0.00000;,
 38.63691;-10.93000;-0.00000;,
 52.90103;-88.20380;-0.00000;,
 42.32090;-88.20380;-0.00000;,
 29.04298;-0.10582;29.04303;,
 27.32039;-10.93000;27.32047;,
 37.40671;-88.20380;37.40671;,
 29.92537;-88.20380;29.92537;,
 0.00000;-0.10582;41.07311;,
 0.00000;-10.93000;38.63691;,
 0.00000;-88.20380;52.90103;,
 0.00000;-88.20380;42.32090;,
 -29.04303;-0.10582;29.04303;,
 -27.32039;-10.93000;27.32047;,
 -37.40671;-88.20380;37.40671;,
 -29.92537;-88.20380;29.92537;;
 
 48;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;1,8,9,2;,
 4;10,4,7,11;,
 4;8,12,13,9;,
 4;14,10,11,15;,
 4;12,16,17,13;,
 4;18,14,15,19;,
 4;16,20,21,17;,
 4;22,18,19,23;,
 4;20,24,25,21;,
 4;26,22,23,27;,
 4;24,28,29,25;,
 4;30,26,27,31;,
 4;28,32,33,29;,
 4;34,30,31,35;,
 4;3,2,36,37;,
 4;7,6,38,39;,
 4;2,9,40,36;,
 4;11,7,39,41;,
 4;9,13,42,40;,
 4;15,11,41,43;,
 4;13,17,44,42;,
 4;19,15,43,45;,
 4;17,21,46,44;,
 4;23,19,45,47;,
 4;21,25,48,46;,
 4;27,23,47,49;,
 4;25,29,50,48;,
 4;31,27,49,51;,
 4;29,33,52,50;,
 4;35,31,51,53;,
 4;0,5,54,55;,
 4;56,57,58,59;,
 4;55,54,60,61;,
 4;57,62,63,58;,
 4;61,60,64,65;,
 4;62,66,67,63;,
 4;65,64,68,69;,
 4;66,70,71,67;,
 4;69,68,72,73;,
 4;70,74,75,71;,
 4;73,72,76,77;,
 4;74,78,79,75;,
 4;77,76,80,81;,
 4;78,82,83,79;,
 4;81,80,5,0;,
 4;82,56,59,83;;
 
 MeshMaterialList {
  3;
  48;
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
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.627451;0.000000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.235200;0.213600;0.235200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.793600;0.800000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  57;
  -0.972092;-0.234599;-0.000000;,
  -0.687373;-0.234599;-0.687373;,
  0.000000;-0.234599;-0.972092;,
  0.687373;-0.234599;-0.687373;,
  0.972092;-0.234598;0.000000;,
  0.687373;-0.234598;0.687373;,
  0.000000;-0.234598;0.972092;,
  -0.687373;-0.234599;0.687373;,
  -0.999469;0.032594;-0.000000;,
  -0.706731;0.032594;-0.706731;,
  0.000000;0.032594;-0.999469;,
  0.706731;0.032594;-0.706731;,
  0.999469;0.032594;-0.000000;,
  0.706731;0.032593;0.706731;,
  0.000000;0.032593;0.999469;,
  -0.706731;0.032593;0.706731;,
  -0.950280;0.311397;0.000000;,
  -0.671949;0.311398;-0.671949;,
  0.000000;0.311397;-0.950280;,
  0.671949;0.311398;-0.671949;,
  0.950280;0.311397;0.000000;,
  0.671949;0.311398;0.671949;,
  0.000000;0.311397;0.950280;,
  -0.671949;0.311398;0.671949;,
  0.903938;0.427664;0.000000;,
  0.639181;0.427664;0.639181;,
  0.000000;0.427664;0.903938;,
  -0.639181;0.427664;0.639181;,
  -0.903938;0.427664;-0.000000;,
  -0.639181;0.427663;-0.639181;,
  -0.000000;0.427664;-0.903938;,
  0.639181;0.427664;-0.639181;,
  0.995550;0.094232;0.000000;,
  0.703960;0.094232;0.703960;,
  0.000000;0.094232;0.995550;,
  -0.703960;0.094232;0.703960;,
  -0.995550;0.094232;-0.000000;,
  -0.703960;0.094231;-0.703960;,
  -0.000000;0.094232;-0.995550;,
  0.703960;0.094232;-0.703960;,
  0.967313;-0.253584;-0.000000;,
  0.683994;-0.253584;0.683994;,
  0.000000;-0.253584;0.967313;,
  -0.683994;-0.253584;0.683994;,
  -0.967313;-0.253584;0.000000;,
  -0.683994;-0.253584;-0.683994;,
  0.000000;-0.253584;-0.967313;,
  0.683994;-0.253584;-0.683994;,
  -0.975595;-0.219576;-0.000000;,
  -0.689850;-0.219577;-0.689850;,
  0.000000;-1.000000;-0.000000;,
  0.000000;-0.219577;-0.975595;,
  0.689850;-0.219577;-0.689850;,
  0.975596;-0.219575;0.000000;,
  0.689850;-0.219573;0.689851;,
  0.000000;-0.219574;0.975596;,
  -0.689850;-0.219575;0.689851;;
  48;
  4;0,1,9,8;,
  4;25,24,32,33;,
  4;1,2,10,9;,
  4;26,25,33,34;,
  4;2,3,11,10;,
  4;27,26,34,35;,
  4;3,4,12,11;,
  4;28,27,35,36;,
  4;4,5,13,12;,
  4;29,28,36,37;,
  4;5,6,14,13;,
  4;30,29,37,38;,
  4;6,7,15,14;,
  4;31,30,38,39;,
  4;7,0,8,15;,
  4;24,31,39,32;,
  4;8,9,17,16;,
  4;33,32,40,41;,
  4;9,10,18,17;,
  4;34,33,41,42;,
  4;10,11,19,18;,
  4;35,34,42,43;,
  4;11,12,20,19;,
  4;36,35,43,44;,
  4;12,13,21,20;,
  4;37,36,44,45;,
  4;13,14,22,21;,
  4;38,37,45,46;,
  4;14,15,23,22;,
  4;39,38,46,47;,
  4;15,8,16,23;,
  4;32,39,47,40;,
  4;0,48,49,1;,
  4;50,50,50,50;,
  4;1,49,51,2;,
  4;50,50,50,50;,
  4;2,51,52,3;,
  4;50,50,50,50;,
  4;3,52,53,4;,
  4;50,50,50,50;,
  4;4,53,54,5;,
  4;50,50,50,50;,
  4;5,54,55,6;,
  4;50,50,50,50;,
  4;6,55,56,7;,
  4;50,50,50,50;,
  4;7,56,48,0;,
  4;50,50,50,50;;
 }
 MeshTextureCoords {
  84;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.125000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.500000;,
  0.125000;0.500000;,
  0.250000;0.000000;,
  0.250000;0.500000;,
  0.250000;0.000000;,
  0.250000;0.500000;,
  0.375000;0.000000;,
  0.375000;0.500000;,
  0.375000;0.000000;,
  0.375000;0.500000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.625000;0.000000;,
  0.625000;0.500000;,
  0.625000;0.000000;,
  0.625000;0.500000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.750000;0.000000;,
  0.750000;0.500000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  0.875000;0.000000;,
  0.875000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  1.000000;0.000000;,
  1.000000;0.500000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.000000;1.000000;,
  0.125000;1.000000;,
  0.250000;1.000000;,
  0.250000;1.000000;,
  0.375000;1.000000;,
  0.375000;1.000000;,
  0.500000;1.000000;,
  0.500000;1.000000;,
  0.625000;1.000000;,
  0.625000;1.000000;,
  0.750000;1.000000;,
  0.750000;1.000000;,
  0.875000;1.000000;,
  0.875000;1.000000;,
  1.000000;1.000000;,
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
  0.000000;0.000000;;
 }
}
