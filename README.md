# RTRT
"real-time" ray-tracer

raymarching techniques are useful for rendering high complexity surfaces where polygon counts would be unusally high with traditional rendering techniques. Here I build what is essentially a glorified linker to compile an array of distance estimated primitives and envionmental attributes (such as lights and domain transform) into a simple glsl fragment shader. "real time" may be a bit of an exaggeration here as such performance can only be expected with a very small number of primitives, and innacurate raytracing settings.
