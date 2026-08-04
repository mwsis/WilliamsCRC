# WilliamsCRC - FAQ <!-- omit in toc -->

The FAQ list is under (constant) development. If you post a question on
[GitHub Issues](https://github.com/synesissoftware/WilliamsCRC/issues) it
will be used to create one.

- [Q1: "How do I build WilliamsCRC?"](#q1-how-do-i-build-williamscrc)
- [Q2: "Why is STLSoft required?"](#q2-why-is-stlsoft-required)


# FAQs: <!-- omit in toc -->


## Q1: "How do I build WilliamsCRC?"

See instructions in [**INSTALL.md**](./INSTALL.md).


## Q2: "Why is STLSoft required?"

The core implementation uses **STLSoft** facilities (e.g. scoped handles and
safe-string helpers). Install **STLSoft** via **CMake**, or point
`STLSOFT` / `--stlsoft-root-dir` at a local tree, as described in
[**INSTALL.md**](./INSTALL.md).


<!-- ########################### end of file ########################### -->
