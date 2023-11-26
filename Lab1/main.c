#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define D 5
#define K 4
#define H1 5
#define H2 5
#define H3 5
#define NUM_LAYERS 3
#define FUNC 0 // 0 for logistic 1 for relu

int neurons_per_layer[3] = {H1,H2,H3};

double generateRandomDouble() {
    
    srand(time(NULL));
    int randInt = rand();
    double randDouble = (randInt / (double)RAND_MAX) * 2.0 - 1.0;

    return randDouble;
}

struct Neuron {
    double bias;
    double *Weights;
    double output;
    int num_input;
};

struct Layer {
    int num_neurons;
    struct Neuron *neurons;
};

struct Neural_Network {
    int num_layers;
    struct Layer *layers;
};

void init_neurons(struct Neuron* neuron){
    neuron->bias = generateRandomDouble();

}

void init_layer(struct Layer* layer, int num_neurons){
    layer->num_neurons = num_neurons;
    layer->neurons = malloc(sizeof(struct Neuron) * num_neurons);

    for(int i = 0; i < layer->num_neurons; i++){
        init_neurons(&layer->neurons[i]);
    }

}

void init_neural_network(struct Neural_Network* network, int num_layers){
    network->num_layers = num_layers;
    network->layers = malloc(sizeof(struct Layer) * num_layers);

    for(int i = 0; i < network->num_layers; i++){
        init_layer(&(network->layers[i]), neurons_per_layer[i]);
    }

}

void forward_pass(float *x, int d, float *y, int k){
    
}

int main(int argc, char* argv[]){

    struct Neural_Network network;
    //init_neural_network()
}