# Load-Balancing Algorithm Simulation

EECE 5643 Load-balancing algorithms simulation.

Created by Hannah Khafif, Tyler Royere, Fizzah Shaikh, & Nicolas Tedori

## Contents

1. [Background](#background)
2. [Files](#files)
3. [Data](#data)
4. [Model](#model)

### Background

This project aims to evaluate multiple load-balancing algorithms on the effect of server utilization of multiple different server models.

### Files

The files in this repo are outlined below

1. [data](./data) (See [here](#data).)
   1. [anon_jobs_2020-08.json.bz2](./data/anon_jobs_2020-08.json.bz2) - this is one month of data from the Eagle supercomputer.
   2. [discovery-data.csv.bz2](./data/discovery-data.csv.bz2) - this is the Discovery cluster data for about 8 months of submitted jobs.
   3. [test_data.json](./data/test_data.json) - primarily used to test the [JSON data parser utility](./util/json-data-parser.py).
   4. [test.csv](./data/test.csv) - a truncated version of the whole discovery dataset, primarily used for testing.
2. [model](./model) (See [here](#model).)
   1. [Dispatcher.cpp](./model/Dispatcher.cpp) and [Dispatcher.h](./model/Dispatcher.h) - this is a class for the model's dispatcher.
   2. 

### Data

This contains all the data files used for the simulation.

Source for the real world data come from NREL's Eagle supercomputer. Link to the data [here](https://data.nrel.gov/submissions/152).

Additionally, data from Northeastern University's Discovery cluster was provided to us. This data is not publicly available. More info available [here](https://rc.northeastern.edu/). This is the `discovery-data.csv.bz2` file, which is a trimmed down version of the original dataset featuring only relevant information necessary for this project.

### Model

These are the C++ files for the simulation to run. Currently (2020-03-12) these files are mostly just a skeleton for a basic implementation.