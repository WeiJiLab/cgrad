#include "neuron.h" 

NEURON *neuron(int num_inputs) {
    NEURON *n = malloc(sizeof(NEURON));
    assert(n != NULL);
    assert(num_inputs > 0);

    n->num_inputs = num_inputs;

    n->params = malloc(sizeof(PARAM) * (num_inputs + 1));
    assert(n->params != NULL);

    /* initializing weights and biases with normal distribution */

    for (int i = 0; i < num_inputs + 1; i++) {
        n->params[i] = (PARAM) {
            .val = normal(0.0, sqrt(2.0 / num_inputs))
        };
    }

    n->weights = malloc(sizeof(VALUE *) * (num_inputs + 1));
    assert(n->weights != NULL);

    for (int i = 0; i < num_inputs + 1; i++) {
        n->weights[i] = parameter(n->params + i);
    }

    return n;
}

VALUE *neuron_forward(NEURON *n, VALUE **x, OPERATION activation) {
    assert(n != NULL);
    assert(x != NULL);
    assert(activation == RELU || activation == TANH || activation == SIGMOID || activation == CONST);

    VALUE *out = constant(0);

    for (int i = 0; i < n->num_inputs; i++) {
        out = add(out, mul(n->weights[i + 1], x[i]));
    }
    out = add(out, n->weights[0]);

    switch (activation) {
        case RELU:
            out = relu(out);
            break;
        case TANH:
            out = tanhyp(out);
            break;
        case SIGMOID:
            out = sigmoid(out);
            break;
        case CONST:
            break;
        default:
            assert(false);
    }

    return out;
}

void neuron_descend(NEURON *n, double lr, bool momentum) {
    assert(n != NULL);
    assert(lr > 0);

    double change;
    double beta = 0.9; // momentum coefficient
    for (int i = 0; i < n->num_inputs + 1; i++) {
        change = lr * n->params[i].grad;

        if (momentum) {
            change += lr * beta * n->params[i].momentum;
            n->params[i].momentum = change;
        }

        n->params[i].val -= change;
    }
}

void free_neuron(NEURON *n) {
    assert(n != NULL);

    free(n->params);
    free(n->weights);
    free(n);

    n = NULL;
}

void zero_grad(NEURON *n) {
    assert(n != NULL);

    for (int i = 0; i < n->num_inputs + 1; i++) {
        n->params[i].grad = 0;
    }
}
