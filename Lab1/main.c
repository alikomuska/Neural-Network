#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define D 2 // number of inputs
#define K 4 // number of outputs
#define MAX_LINES 8000 
#define H1 20 // hidden layer 2000
#define H2 20 // ... 2
#define H3 20 // ... 3
#define NUM_LAYERS 3
#define FUNC 1 // 0 for logistic 1 for hyberbolic 2 for relu
#define B 40


// 3 hidden layers and one output layer
int neurons_per_layer[4] = {H1,H2,H3, K};

float generateRandomfloat() {
    
    int randInt = rand();
    float randfloat = (randInt / (float)RAND_MAX) * 2.0 - 1.0;

    return randfloat;
}

float sigmoid(float x) {

    float result = 1.0 / (1.0 + exp(-x));

    return result;
}

float relu(float x) {

    float result = fmax(0, x);

    return result;
}

float hyperbolic(float x) {

    float result = (exp(x) - exp(-x)) / (exp(x) + exp(-x));

    return result;
}

float linear(float x) {

    return x;
}


struct Neuron {
    float bias;
    float *Weights;
    float *error_derivative;
    float weighted_sum;
    float output;
    int layer;
    int num_input;
    float error;
    float derivative;
    float bias_derivative;

};

struct Layer {
    int num_neurons;
    int layer_no;
    struct Neuron *neurons;
};

struct Neural_Network {
    int num_layers;
    struct Layer *layers;
    float total_error;
};

void loadDataset(const char *filename, float x[][D], int *numExamples) {
    FILE *file;
    file = fopen(filename, "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        *numExamples = 0;
        return;
    }

    int line_count = 0;

    while (fscanf(file, "%f %f", &x[line_count][0], &x[line_count][1]) == 2) {
        // Successfully read two values from the line
        line_count++;

        if (line_count >= MAX_LINES) {
            printf("Exceeded maximum number of lines. Increase MAX_LINES.\n");
            break;
        }
    }

    fclose(file);

    *numExamples = line_count;
}




void init_neurons(struct Neuron* neuron, int layer){
    neuron->bias = generateRandomfloat();
    neuron->layer = layer;
    neuron->bias_derivative = 0;

    //printf("MY LAYER IS %d\n", layer);
    

    if(layer == 1){
        neuron->num_input = D;
        //neuron->Weights = malloc(sizeof(float) * neuron->num_input);
        //neuron->error_derivative = malloc(sizeof(float) * neuron->num_input);
    }else{
        neuron->num_input = neurons_per_layer[layer-2];
        //neuron->Weights = malloc(sizeof(float) * neuron->num_input);
        //neuron->error_derivative = malloc(sizeof(float) * neuron->num_input);
    }

    neuron->Weights = malloc(sizeof(float) * neuron->num_input);
    neuron->error_derivative = malloc(sizeof(float) * neuron->num_input);

    for(int i = 0; i < neuron->num_input; i++){
        neuron->Weights[i] = generateRandomfloat();
        neuron->error_derivative[i] = 0;
    }
    
}

void init_layer(struct Layer* layer, int num_neurons, int layer_no){
    layer->num_neurons = num_neurons;
    layer->layer_no = layer_no + 1;
    layer->neurons = malloc(sizeof(struct Neuron) * num_neurons);

    for(int i = 0; i < layer->num_neurons; i++){
        //printf("Layer num %d\n", layer_no);
        init_neurons(&layer->neurons[i], layer->layer_no);
    }

}

void init_neural_network(struct Neural_Network* network, int num_layers){
    network->num_layers = num_layers + 1;
    network->layers = malloc(sizeof(struct Layer) * network->num_layers);
    
    for(int i = 0; i < num_layers; i++){
        init_layer(&(network->layers[i]), neurons_per_layer[i], i);
    }

    init_layer(&(network->layers[num_layers]), neurons_per_layer[num_layers], num_layers);

}

void print_network(struct Neural_Network* network) {
    printf("Neural Network Architecture:\n");

    for (int i = 0; i < network->num_layers; i++) {
        printf("\nLayer %d:\n", i + 1);
        printf("Number of neurons: %d\n", network->layers[i].num_neurons);

        for (int j = 0; j < network->layers[i].num_neurons; j++) {
            printf("\nNeuron %d:\n", j + 1);
            printf("Bias: %lf\n", network->layers[i].neurons[j].bias);
            printf("Number of input neurons: %d\n", network->layers[i].neurons[j].num_input);
            printf("Error: %lf\n", network->layers[i].neurons[j].error);
            printf("Weights:\n");
            for (int k = 0; k < network->layers[i].neurons[j].num_input; k++) {
                printf("  Weight %d: %lf\n", k + 1, network->layers[i].neurons[j].Weights[k]);
            }
            printf("--------------------------------------------\n");
            for (int k = 0; k < network->layers[i].neurons[j].num_input; k++) {
                printf(" Error Weight %d: %lf\n", k + 1, network->layers[i].neurons[j].error_derivative[k]);
            }
            printf("Derivative of bias: %lf\n", network->layers[i].neurons[j].bias_derivative);
          
            printf("print the output %lf \n", network->layers[i].neurons[j].output);
        }
    }

    printf("--------------------------------------------\n");
    printf("total error: %lf", network->total_error);
    printf("\n");
}

void classification(float *x, float *t) {
    t[0] = t[1] = t[2] = t[3] = 0;  // Initialize array elements
    int flag = 0;

    if((pow((x[0] - 0.5) , 2) + pow((x[1] - 0.5) , 2)) < 0.2 && x[0] > 0.5){
        t[0] = 1;
        flag = 1;
    } else if((pow((x[0] - 0.5) , 2) + pow((x[1] - 0.5) , 2)) < 0.2 && x[0] < 0.5) {
        t[1] = 1;
        flag = 1;
    } else if((pow((x[0] + 0.5) , 2) + pow((x[1] + 0.5) , 2)) < 0.2 && x[0] > -0.5) {
        t[0] = 1;
        flag = 1;
    } else if((pow((x[0] + 0.5) , 2) + pow((x[1] + 0.5) , 2)) < 0.2 && x[0] < -0.5) {
        t[1] = 1;
        flag = 1;
    } else if((pow((x[0] - 0.5) , 2) + pow((x[1] + 0.5) , 2)) < 0.2 && x[0] > 0.5) {
        t[0] = 1;
        flag = 1;
    } else if((pow((x[0] - 0.5) , 2) + pow((x[1] + 0.5) , 2)) < 0.2 && x[0] < 0.5) {
        t[1] = 1;
        flag = 1;
    } else if((pow((x[0] + 0.5) , 2) + pow((x[1] - 0.5) , 2)) < 0.2 && x[0] > -0.5) {
        t[0] = 1;
        flag = 1;
    } else if((pow((x[0] + 0.5) , 2) + pow((x[1] - 0.5) , 2)) < 0.2 && x[0] < -0.5) {
        t[1] = 1;
        flag = 1;
    }

    if (flag == 0 && x[0] > 0) {
        t[2] = 1;
        flag = 1;
    }else if(flag == 0 && x[0] < 0) {
        t[3] = 1;
    }
}


void forward_pass(struct Neural_Network* network, float *x, int d, float *y, int k){
    int i = 0;
    float *input_vector = NULL;
    float *output_vector = NULL;
    
    for (i; i < NUM_LAYERS + 1; i++) {
        
        if (i == 0) {
            // If we're in the first hidden layer, the input vector is the x vector
            input_vector = malloc(sizeof(float) * d);
            for (int l = 0; l < d; l++) {
                input_vector[l] = x[l];
            }
        } else {
            // Free the output_vector from the previous layer
            free(output_vector);

            // Free the output_vector from the previous layer
            free(input_vector);
            
            // Allocate memory for the input_vector of the current layer
            input_vector = malloc(sizeof(float) * network->layers[i-1].num_neurons);
            
            // Copy values from the output_vector of the previous layer to the input_vector
            for (int l = 0; l < network->layers[i-1].num_neurons; l++) {
                input_vector[l] = network->layers[i-1].neurons[l].output;
            }
        }
        
        // Allocate memory for the output_vector of the current layer
        output_vector = malloc(sizeof(float) * network->layers[i].num_neurons);
        
        for (int j = 0; j < network->layers[i].num_neurons; j++) {
            float weighted_sum = 0;
            // Reset the weighted sum for each neuron
            network->layers[i].neurons[j].weighted_sum = 0.0;
            for (int k = 0; k < network->layers[i].neurons[j].num_input; k++) {
                network->layers[i].neurons[j].weighted_sum += input_vector[k] * network->layers[i].neurons[j].Weights[k]; 
            }
            network->layers[i].neurons[j].weighted_sum += network->layers[i].neurons[j].bias;

            if(i == NUM_LAYERS) {
                // linear for output layer
                network->layers[i].neurons[j].output = sigmoid(network->layers[i].neurons[j].weighted_sum);
            }else {
                // hidden layers activation functions
                if (FUNC == 0) {
                    // Logistic (Sigmoid) Activation Function
                    network->layers[i].neurons[j].output = sigmoid(network->layers[i].neurons[j].weighted_sum);
                } else if(FUNC == 1) {
                    // hyperbolic Activation Function
                    network->layers[i].neurons[j].output = hyperbolic(network->layers[i].neurons[j].weighted_sum);
                } else {
                    // relu
                    network->layers[i].neurons[j].output = relu(network->layers[i].neurons[j].weighted_sum);
                }
            }
            // Copy the values of each neuron to the output vector 
            output_vector[j] = network->layers[i].neurons[j].output;
        }
    }

    // Calculate output layer vector y with k
    for (int l = 0; l < k; l++) {
        y[l] = output_vector[l]; 
    }

    // Free memory allocated for input and output vectors
    free(input_vector);
    free(output_vector);
}

void backprop(struct Neural_Network *network, float *x, int d, float *t, int k) {

    float *output = malloc(sizeof(float) * K);

    forward_pass(network, x , d, output, K);

    // compute error
    float error = 0;
    for(int i = 0; i < K; i++){
        error += pow(t[i] - output[i] , 2);
    }

    error = error /2;
    network->total_error = error;

    //compute errors in each neuron backwards

    for(int i = NUM_LAYERS; i >= 0; i--){
        for(int j = 0; j < network->layers[i].num_neurons; j++){
            if(i == NUM_LAYERS) {
                network->layers[i].neurons[j].error = network->layers[i].neurons[j].output * (1 - network->layers[i].neurons[j].output) *
                ( network->layers[i].neurons[j].output- t[j]);
            }else {
                float error_accumulator = 0.0;
                for(int k = 0; k < network->layers[i + 1].num_neurons; k++){
                         error_accumulator += network->layers[i + 1].neurons[k].Weights[j] *
                                     network->layers[i + 1].neurons[k].error;
                }
                if(FUNC == 0) {
                    network->layers[i].neurons[j].error = network->layers[i].neurons[j].output *
                                                   (1 - network->layers[i].neurons[j].output) *
                                                   error_accumulator;
                } else if(FUNC == 1){
                    network->layers[i].neurons[j].error = (1 - pow(network->layers[i].neurons[j].output, 2)) * error_accumulator;
                } else {
                    float relu_der = 0;
                    if(network->layers[i].neurons[j].output > 0) {
                        relu_der = 1;
                    } else {
                        relu_der = 0;
                    }
                    network->layers[i].neurons[j].error = relu_der * error_accumulator;

                }
                
            }

            if(i == 0){
                // compute error derivatives
                for(int l = 0; l < network->layers[i].neurons[j].num_input; l++){
                    network->layers[i].neurons[j].error_derivative[l] += network->layers[i].neurons[j].error * x[l];
                }
            }else{
                for(int l = 0; l < network->layers[i].neurons[j].num_input; l++){
                    network->layers[i].neurons[j].error_derivative[l] += network->layers[i].neurons[j].error * network->layers[i - 1].neurons[l].output;
                }
            }
            network->layers[i].neurons[j].bias_derivative += network->layers[i].neurons[j].error;

        }
    }

}






int main(int argc, char* argv[]) {

    srand(time(NULL));

    // Load dataset
    float x[MAX_LINES][D];
    int numExamples;
    loadDataset("dataset.txt", x, &numExamples);

    // Initialize neural network
    struct Neural_Network network;
    init_neural_network(&network, NUM_LAYERS);

    // Training parameters
    int epochs = 800;
    float learning_rate = 0.001;

    // Training loop
    for (int epoch = 0; epoch < epochs; epoch++) {
        int batch = 0;
        for (int example = 0; example < 4000; example++) {
            batch++;
            // Input and target for the selected example
            float input[D];
            float target[K];
            for (int i = 0; i < D; i++) {
                input[i] = x[example][i];
            }
            classification(input, target);

            // Perform forward pass and backpropagation
            backprop(&network, input, D, target, K);

            // Update weights and biases using batch update

            if(batch == B){
                for (int i = 0; i < network.num_layers; i++) {
                    for (int j = 0; j < network.layers[i].num_neurons; j++) {
                        for (int k = 0; k < network.layers[i].neurons[j].num_input; k++) {
                            network.layers[i].neurons[j].Weights[k] -= learning_rate * network.layers[i].neurons[j].error_derivative[k];
                            network.layers[i].neurons[j].error_derivative[k] = 0;
                        }
                        network.layers[i].neurons[j].bias -= learning_rate * network.layers[i].neurons[j].bias_derivative;
                        network.layers[i].neurons[j].bias_derivative = 0;
                    }
                }
                batch = 0;
            }
        }

        // Print the total error for every 100 epochs
        if (epoch % 1 == 0) {
            printf("Epoch %d - Total Error: %lf\n", epoch, network.total_error);
        }
    }

    
    int count = 0;

    for (int example = 4000; example < 8000; example++) {
        float input[D];
        float target[K];

        for (int i = 0; i < D; i++) {
            input[i] = x[example][i];
        }
        classification(input, target);

        float output[K];
        forward_pass(&network, input, D, output, K);

        int highest_value_index = 0;
        float highest_value = output[0];

        // Find the index of the highest value in the output array
        for (int i = 1; i < K; i++) {
            if (output[i] > highest_value) {
                highest_value = output[i];
                highest_value_index = i;
            }
        }

        // Check if the highest value index matches the target index
        if (target[highest_value_index] == 1) {
            count++;
        }   
    }

    float accuracy = count / 4000.0;
    printf("Accuracy: %f%%\n", accuracy * 100.0);

    return 0;
}