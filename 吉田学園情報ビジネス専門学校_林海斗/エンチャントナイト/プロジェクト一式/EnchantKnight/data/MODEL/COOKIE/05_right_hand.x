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
 -1.40898;5.35873;-3.43432;,
 1.17391;1.79820;-3.43432;,
 -13.82730;2.93568;-3.43432;,
 -9.60959;5.35882;-3.43432;,
 1.17391;-1.76242;-3.43432;,
 -13.82730;-2.79469;-3.43432;,
 -1.40898;-5.32300;-3.43432;,
 -9.60961;-5.32299;-3.43432;,
 -1.40898;-5.32300;-3.43432;,
 -1.40898;-5.32300;3.44618;,
 -9.60961;-5.32299;3.44618;,
 -9.60961;-5.32299;-3.43432;,
 -1.40898;-5.32300;3.44618;,
 1.17391;-1.76242;3.44618;,
 -13.82730;-2.79469;3.44618;,
 -9.60961;-5.32299;3.44618;,
 1.17391;1.79820;3.44618;,
 -13.82730;2.93568;3.44618;,
 -1.40898;5.35873;3.44618;,
 -9.60959;5.35882;3.44618;,
 -1.40898;5.35873;3.44618;,
 -1.40898;5.35873;-3.43432;,
 -9.60959;5.35882;-3.43432;,
 -9.60959;5.35882;3.44618;,
 1.17391;1.79820;3.44618;,
 1.17391;1.79820;-3.43432;,
 1.17391;-1.76242;3.44618;,
 1.17391;-1.76242;-3.43432;,
 1.17391;1.79820;-3.43432;,
 -13.82730;2.93568;-3.43432;,
 -13.82730;2.93568;3.44618;,
 -13.82730;-2.79469;-3.43432;,
 -13.82730;-2.79469;3.44618;;
 
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
 4;24,26,27,28;,
 4;26,9,8,27;,
 4;22,29,30,23;,
 4;29,31,32,30;,
 4;31,11,10,32;;
 
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
  11;
  0.000000;0.000000;-1.000000;,
  0.454320;-0.890839;-0.000000;,
  0.000000;0.000000;1.000000;,
  -0.266740;-0.963769;0.000000;,
  0.454322;0.890838;0.000000;,
  -0.257786;0.966202;0.000000;,
  0.951170;0.308668;0.000000;,
  0.951171;-0.308665;-0.000000;,
  -0.498156;0.867087;0.000000;,
  -1.000000;0.000000;0.000000;,
  -0.870100;-0.492875;0.000000;;
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
  4;9,10,10,9;,
  4;10,3,3,10;;
 }
 MeshTextureCoords {
  33;
  0.406852;2.118712;,
  0.577608;2.354101;,
  -0.414129;2.278902;,
  -0.135294;2.118707;,
  0.577608;2.589495;,
  -0.414129;2.657739;,
  0.406852;2.824887;,
  -0.135295;2.824886;,
  0.938037;1.659650;,
  0.561770;1.659650;,
  0.695198;1.659649;,
  0.804628;1.659649;,
  0.406852;2.824887;,
  0.577608;2.589495;,
  -0.414129;2.657739;,
  -0.135295;2.824886;,
  0.577608;2.354101;,
  -0.414129;2.278902;,
  0.406852;2.118712;,
  -0.135294;2.118707;,
  0.561770;1.125563;,
  0.938037;1.125563;,
  0.804628;1.125559;,
  0.695198;1.125559;,
  0.447747;1.303590;,
  1.052420;1.303590;,
  0.447747;1.481621;,
  0.052420;1.481621;,
  0.052420;1.303590;,
  0.788746;1.246716;,
  0.711126;1.246716;,
  0.788746;1.533234;,
  0.711126;1.533234;;
 }
}
