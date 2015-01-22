# RTRT
"real-time" ray-tracer


the following sceenshots were taken my laptop running on an intel 4000 graphics card with OpenGL 3.3, mesa 10.4.2, glsl version 1.30. these demos ran in 1080p at 30 and 7 fps respectively.

![alt tag](https://raw.githubusercontent.com/patrick38894/RTRT/master/Screenshot%20-%2001202015%20-%2001:45:37%20PM.png)

![alt tag](https://raw.githubusercontent.com/patrick38894/RTRT/master/Screenshot%20-%2001212015%20-%2004:08:57%20PM.png)



raymarching techniques are useful for rendering high complexity surfaces where polygon counts would be unusally high with traditional rendering techniques. Here I build what is essentially a glorified linker to compile an array of distance estimated primitives and envionmental attributes (such as lights and domain transform) into a simple glsl fragment shader. "real time" may be a bit of an exaggeration here as such performance can only be expected with a very small number of primitives, and innacurate raytracing settings.
