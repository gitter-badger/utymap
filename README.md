[![Build Status](https://travis-ci.org/reinterpretcat/utymap.svg?branch=master)](https://travis-ci.org/reinterpretcat/utymap)
<h2> Description </h2>

UtyMap is library for building real city environment dynamically using various data sources (mostly, OpenStreetMap and Natural Earth). It includes:
* rendering of different models (e.g. buildings, roads, parks, rivers, POI, etc.) using map data for given location on the fly by terrain tiling approach and different zoom levels
* easy customization of rendered models using mapcss file
* non-flat terrain with Data Elevation Model (SRTM) files
* modification of city environment (terrain craters, corrupt buildings, etc..)[**planned**]

The library can be used to build different 3D-games (like car simulations or GTA 2/3 ) or some map tools. Target platforms are planned as all popular mobile OS and VR devices supported by Unity3D.

<h2> Project structure </h2>
Project consists of two sub-projects:
<ul>
    <li><b>core</b> contains essential logic written on C++11 to build library for constructing of map based apps: terrain/buildings/osm-objects mesh generators, mapcss parser, spatial geo index, etc. </li>
    <li><b>unity</b> contains examples written on C# which can be reused to build map oriented Unity apps using core library. It will demonstrate basic use cases: globe zoom level rendering, 3D scene with all details.</li>
</ul>

<h2> Project status </h2>
<p> Project is under development, more details will be given later. </p>

<h2> Install </h2>
Core library has some dependencies. The following versions are used in development environment:
<ul>
    <li> <b>CMake 3.1 or higher.  </b> Some specific flags (CMAKE_CXX_STANDARD, POSITION_INDEPENDENT_CODE) are used. </li>
    <li> <b>Boost 1.55 or higher. </b> Boost is used only for parsing mapcss, xml deserialization and unit testing.  </li>
    <li> <b> Protobuf library and compiler 2.6.1 or other compatible. </b> Protobuf is used for parsing osm pbf files.</li>
    <li> <b> zlib 1.2.8 or other compatible. </b> Used for osm pbf files decompression. </li>
</ul>

OS specific instructions can be found here:
* [Linux](https://github.com/reinterpretcat/utymap/wiki/Install-on-Linux)
* [Windows](https://github.com/reinterpretcat/utymap/wiki/Install-on-windows)


[![Join the chat at https://gitter.im/reinterpretcat/utymap](https://badges.gitter.im/reinterpretcat/utymap.svg)](https://gitter.im/reinterpretcat/utymap?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)