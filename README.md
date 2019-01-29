# Tracer
This project is a path tracer. It is developed in Qt. Multi-importance sampling, ruassian roulette termination, micro-facet material, fresnel effect, constructive solid geometry and signed distance field and an experimental photon mapper are implemented.

---

* ### Lens Based Camera 

###### Fulllight Integrator 1024x1024 pixels 20x20 samples 5 maximum recursions 1.5 hours

![](1_1024_20_5_1.5h.png)

---

* ### Lens Based Camera

* ### BVH + KD Tree

* ### Texture

* ### Normal Map

###### Fulllight Integrator 1024x1024 pixels 20x20 samples 5 maximum recursions 6 hours

![](2_1024_20_5_6h.png)

###### (N64 Catridge model made by Chris Sweetwood @ sketchfab)

---

* ### BVH + KD Tree

* ### Chrome Material

###### Fulllight Integrator 1024x1024 pixels 20x20 samples 5 maximum recursions 63.5 hours

![](3_1024_20_5_63.5h.png)

###### (Metal Gear Rex model made by me)

---

* ### BVH + KD Tree

* ### Texture 

* ### Normal Map

* ### Chrome Material

###### Fulllight Integrator 1024x1024 pixels 20x20 samples 5 maximum recursions 7.8 hours

![](4_1024_20_5_7.8h.png)

###### (Game Boy model made by danny_p3d @ sketchfab)

---

* ### Implicit Surface

* ### Chrome Material

###### Fulllight Integrator 400x400 pixels 10x10 samples 5 maximum recursions 5.6 hours

![](5_400_10_5_5.6h.png)

---

* ### Implicit Surface

* ### Chrome Material

###### Fulllight Integrator 400x400 pixels 20x20 samples 5 maximum recursions 60 hours

![](5_400_20_5_60h.png)

---

* ### Contructive Solid Geometry

* ### Difference and Union

###### Fulllight Integrator 400x400 pixels 20x20 samples 5 maximum recursions 1 minute

![](CSG_difference_union.png)

---

* ### Constructive Solid Geometry

* ### Intersection and Union

###### Fulllight Integrator 400x400 pixels 20x20 samples 5 maximum recursions 1 minute

![](CSG_intersection_union.png)

---

* ### Multi-Importance Sampling

Directlight Integrator 768x512 pixels 10x10 samples

|          Without MIS         |             With MIS            |
|:----------------------------:|:-------------------------------:|
|![](veach100SamplesDirect.png)|![](veach100SamplesMISDirect.png)|

---

* ### Directional Light

###### Fulllight Integrator 400x400 pixels 20x20 samples 5 maximum recursions 10 minutes

![](DirectionalLight.png)

---

* ### Environment Light

###### Fulllight Integrator 400x400 pixels 20x20 samples 5 maximum recursions 1 minute

![](EnvironmentLight.png)

---

* ### Unfinished Photon Mapper

###### Photon Integrator 500,000 photons emitted, sampling radius 1.9

![](photon_500000_1.9.png)

###### Photon Integrator 2,000,000 photons emitted, sampling radius 1.8

![](photon_2000000_1.8.png)

###### Photon Integrator 100,000 photons emitted, sampling radius 0.8

|    Caustic Photon Map     |     Indirect Photon Map    |     Path Tracer Reference    |
|:-------------------------:|:--------------------------:|:----------------------------:|
|![](caustic_photon_map.png)|![](indirect_photon_map.png)|![](path_tracer_reference.png)|