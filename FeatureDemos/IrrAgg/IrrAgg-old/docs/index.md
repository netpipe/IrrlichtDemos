# IrrAgg
A collection of utilities for using Anti-Grain Geometry with Irrlicht.

Anti-Grain Geometry (AGG) is a collection of C++ utilities that work together to render vector graphics, especially into arrays of primitive pixels.

Irrlicht is an easy-to-use 2D and 3D rendering engine, created with C++ and aimed at game creation.

IrrAgg is a collection of utilities that enable the easy application of AGG functionality to Irrlicht image surfaces with little or no knowledge of the workings of AGG on the part of the user.

Most functionality is applied to the IImage implementation (CImage) of Irrlicht so that they can be rendered in Irrlicht.

- [API](api)
- [Usage Guide](usage)
- [Examples](examples)

## Requirements

- Anti-Grain Geometry (2.5 or similar)
- Irrlicht (1.8, 1.9 or similar)

Both Anti-Grain Geometry (completed) and Irrlicht (development) are relatively stable in regards to the relevant APIs, so multiple versions of these two libraries will likely be compatible.

## Project layout

    changelog.txt # Change log.
    license.txt   # Licensing.
    Readme.md     # Project summary page (for Github).
    mkdocs.yml    # Documentation configuration file.
    include/      # Header files directory.
    src/          # Source code directory.
    docs/         # Documentation directory.
    debug/        # Debugging files directory.
    examples/     # Usage examples directory.

## Further Information

More information about using AGG with Irrlicht can be found in [this blog article](https://magicsnippet.wordpress.com/2017/12/03/rendering-anti-grain-vector-graphics-in-irrlicht/).
