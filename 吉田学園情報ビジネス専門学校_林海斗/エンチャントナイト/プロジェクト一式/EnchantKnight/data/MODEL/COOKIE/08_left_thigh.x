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
 33;
 -5.33149;-1.01861;-3.43432;,
 -1.77090;1.56427;-3.43432;,
 -2.20103;-14.61020;-3.43432;,
 -5.33149;-11.86993;-3.43432;,
 1.78970;1.56427;-3.43432;,
 2.11570;-14.61020;-3.43432;,
 5.35031;-1.01861;-3.43432;,
 5.35031;-11.86993;-3.43432;,
 5.35031;-1.01861;-3.43432;,
 5.35031;-1.01861;3.44618;,
 5.35031;-11.86993;3.44618;,
 5.35031;-11.86993;-3.43432;,
 5.35031;-1.01861;3.44618;,
 1.78970;1.56427;3.44618;,
 2.11570;-14.61020;3.44618;,
 5.35031;-11.86993;3.44618;,
 -1.77090;1.56427;3.44618;,
 -2.20103;-14.61020;3.44618;,
 -5.33149;-1.01861;3.44618;,
 -5.33149;-11.86993;3.44618;,
 -5.33149;-1.01861;3.44618;,
 -5.33149;-1.01861;-3.43432;,
 -5.33149;-11.86993;-3.43432;,
 -5.33149;-11.86993;3.44618;,
 -1.77090;1.56427;3.44618;,
 -1.77090;1.56427;-3.43432;,
 1.78970;1.56427;3.44618;,
 1.78970;1.56427;-3.43432;,
 -2.20103;-14.61020;-3.43432;,
 -2.20103;-14.61020;3.44618;,
 2.11570;-14.61020;-3.43432;,
 2.11570;-14.61020;3.44618;,
 2.11570;-14.61020;-3.43432;;
 
 14;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;13,16,17,14;,
 4;16,18,19,17;,
 4;20,21,22,23;,
 4;20,24,25,21;,
 4;24,26,27,25;,
 4;26,9,8,27;,
 4;22,28,29,23;,
 4;28,30,31,29;,
 4;32,11,10,31;;
 
 MeshMaterialList {
  5;
  14;
  0,
  0,
  0,
  1,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/cookie000.png";
   }
  }
  Material {
   0.501600;0.408000;0.119200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/cookie000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/cookie_icing000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/button_icing000.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data/TEXTURE/ribbon_icing000.png";
   }
  }
 }
 MeshNormals {
  10;
  0.000000;0.000000;-1.000000;,
  0.890837;0.454323;0.000000;,
  0.000000;0.000000;1.000000;,
  0.907302;-0.420480;-0.000000;,
  -0.890838;0.454322;0.000000;,
  -0.910675;-0.413125;0.000000;,
  -0.308663;0.951171;0.000000;,
  0.308662;0.951172;0.000000;,
  -0.351821;-0.936067;0.000000;,
  0.344235;-0.938883;-0.000000;;
  14;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;1,1,3,3;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;2,2,2,2;,
  4;4,4,5,5;,
  4;4,6,6,4;,
  4;6,7,7,6;,
  4;7,1,1,7;,
  4;5,8,8,5;,
  4;8,9,9,8;,
  4;9,3,3,9;;
 }
 MeshTextureCoords {
  33;
  0.147533;2.540321;,
  0.382925;2.369566;,
  0.354489;3.438868;,
  0.147533;3.257707;,
  0.618318;2.369566;,
  0.639870;3.438868;,
  0.853711;2.540321;,
  0.853711;3.257707;,
  0.159178;1.444430;,
  0.341072;1.444430;,
  0.341072;1.986997;,
  0.159178;1.986997;,
  0.853711;2.540321;,
  0.618318;2.369566;,
  0.639870;3.438868;,
  0.853711;3.257707;,
  0.382925;2.369566;,
  0.354489;3.438868;,
  0.147533;2.540321;,
  0.147533;3.257707;,
  0.658672;1.444430;,
  0.841078;1.444430;,
  0.841078;1.986997;,
  0.658672;1.986997;,
  0.575548;1.315286;,
  0.924229;1.315286;,
  0.423766;1.315286;,
  0.076458;1.315286;,
  0.909290;2.124010;,
  0.590461;2.124010;,
  1.087875;2.124010;,
  0.412370;2.124010;,
  0.087875;2.124010;;
 }
}
