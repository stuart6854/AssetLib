# Custom Binary Model Format Specification

## Header
Allows the file to be identified.

```
Header
{
    signature   U24     // Will represent the string "RES" or 0x52 0x45 0x53 in bytes
    version     U8      // The version of the file format in bytes. Version 1 will be 0x01
}
```

## Mesh Data

```

```

## Material Data

```
MaterialDef
{
    name            STRING
    diffuse         U8[3]
    metallic        FLOAT
    specular        FLOAT
    diffuseMap      U64
}

MaterialData
{
    materialCount   U8
    materials       MaterialDef[materialCount]
}
```
