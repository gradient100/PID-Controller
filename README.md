# PID Controller

Vinh Nghiem

Self-Driving Car Engineer Program
---

## Project Overview

In this project, I use a PID controller to steer a car around a track in a simulator.  The PID parameters are coursely tuned manually and then finely tuned using the twiddle, or coordinate ascent, algorithm.  

## Results and Reflections

This project tuned parameters that were suitable to successfully steer the car around a simulator track. A description of each parameter, its influence on steering behavior, and how each was tuned :

* P, or proportional, component  - steers the car with an angle proportional in the opposite direction to the measured cross-track error (cte), in this case, the perpendicular distance of the car to the midline of the road.  The further the car is from the center of the road, the greater the steering angle.  This component alone, however, will lead to oscillations along the midline of the road, because when the car is at the midline, it will almost always be oriented unparallel to the direction of the road, calculate a 0 steering angle at the midline, and hence will keep its current orientation, while drifting further away from the midline.  Another pitfall of the proportional component in isolation, is that if the car is far from the midline of the road, the calculated steering angle will be large, and the car could run in circles wildly around the midline of the road.

The p component was first tuned by hand.  This was necessary because, otherwise, using an optimization algorithm like twiddle by starting with a 0 value or initializing to a randomized value, would cause the car to fall off the track and the program hangs.  The p component was tuned by hand by incrementing in steps of 0.1 to make sure it at least stayed on the road.  Magnitudes for the p component that were too high caused the car to swing sharply while values that were too low caused the car to not turn enough.  I settled on a manual value for the p component of -0.2 before sending it off to automatic parameter optimization using twiddle.  Twiddle, then, found an optimal p component of -0.18.

* i, or the integral, component - steers the car proportional in the opposite direction to any systemic steering error, such as a steering misalignment caused by the car hitting a rock anytime during its simulation run, for example, will cause the car to drift predominantly more toward one side of the road, and require a higher steering angle in the opposite direction to offset the steering misalignment.  The integral component, then, is the steering angle that offsets the accumulated car drift from the road's midline, over time.

The i component was also first tuned by hand, and later automatically with twiddle.  The i component was first left at 0, while the other 2 components were manually tuned.  This was ok for about 185 runs around the same track, but then the car fell off the track.  I concluded that if the car is going around the same track but falling off many iterations later, something about the car, such as its alignment must be changing.  This could be due to simulated running over of rocks, or other causes of wheel misalignment, by the simulator.  I then increased the i component value very gradually, in increments of 0.001.  Magnitudes that were too high amplified the history of cross-track error of the car and amplified swinging and oscillation by the car.  I setted on a manual value of 0.005 for the i component, before sending it off to automatic parameter optimization using twiddle.  Twiddle, then, found an optimal icomponent of -0.0054965.

* d, or the derivative, component - steers the car proportional in the opposite direction to how fast the cross-track error is changing, which is equivalent here to how fast the steering angle is changing.  The sharper the car is turning into the road, the greater in magnitude the derivative error will be.  This component serves to help dampen the oscillations as the car approaches the midline of the road while turning into the curve.  As the car steers into the midline of the curve, steering angle is increasing while driving into the curve, so the cte is increasing, and so the derivative of the cte is positive. The derivative component then will be a resulting steering angle that is proportional to this positive cte derivative in the opposite direction, or contributing negatively to the calculated steering angle.  Therefore, the derivative component dampens the oscillations caused by the p component.

The d component was also first tuned by hand, and later automatically with twiddle.  The d component was tuned by hand by incrementing in steps of 0.5 to make sure it at least stayed on the road.  Magnitudes for the d component that were too high caused overdampening and the car failed to steer sharply enough into turns for the same value of the p component. Magnitudes too low caused the car to oscillate along the midline of the road.  I settled on a manual value for the d component of -0.4 before sending it off to automatic parameter optimization using twiddle.  Twiddle, then, found an optimal p component of -3.64.

[Here](http://github.com/gradient100/PID-Controller/blob/master/demo.mov) is a video of the car with the final parameters.


## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid`. 

Tips for setting up your environment can be found [here](https://classroom.udacity.com/nanodegrees/nd013/parts/40f38239-66b6-46ec-ae68-03afd8a601c8/modules/0949fca6-b379-42af-a919-ee50aa304e6a/lessons/f758c44c-5e40-4e01-93b5-1a82aa4e044f/concepts/23d376c7-0195-4276-bdf0-e02f1f3c665d)

