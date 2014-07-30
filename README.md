Quick setup
===========

...under construction... we're currently using ROS hydro and a catkin-based setup.

    mkdir -p /path/to/workspace/src
    cd /path/to/workspace/src
    git clone git@github.com:poftwaresatent/vasco_npm.git
    git clone https://github.com/poftwaresatent/sfl2.git
    catkin_init_workspace
    cd ..
    catkin_make

Nepumuk simulator
-----------------

The Nepumuk simulator comes bundled with [Sunflower][sfl2]. You can
test it by running one of the older demos that do not depend on ROS,
e.g.

    ./devel/lib/sfl2/nepumuk -c src/sfl2/apps/expo1.yaml

In the simulator, press SPACE for step-by-step mode, 'c' for
continuous simulation, and 'q' to quit.

In order to run the Vasco setup, we need to tell Nepumuk where to find
the plugin.  Assuming you are at the top of the catkin workspace,
that'll look as follows (your paths may need to be adapted).

    roscore
    NPM_PLUGIN_PATH=devel/lib ./devel/lib/sfl2/nepumuk -c src/vasco_npm/config/vasco.yaml
    rostopic echo /vasco/scan

Adding your own ROS nodes
-------------------------

Just create them inside the `src` directory beside the `sfl2` and
`vasco_npm` directories.

