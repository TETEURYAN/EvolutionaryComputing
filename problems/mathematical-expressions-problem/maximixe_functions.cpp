#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Tamanho da população
const int POPULATION_SIZE = 10;

// Tamanho do cromossomo (número de bits)
const int CHROMOSOME_SIZE = 5;

// Número máximo de gerações
const int MAX_GENERATIONS = 100;

// Taxa de crossover
const double CROSSOVER_RATE = 0.7;

// Taxa de mutação
const double MUTATION_RATE = 0.01;

// Função de aptidão
int fitness(int x) {
    return x * x;
}

// Gerar um número aleatório entre 0 e 1
double randomDouble() {
    return (double)rand() / RAND_MAX;
}

// Gerar um número inteiro aleatório entre 0 e 31
int randomInt() {
    return rand() % 32;
}

// Converter cromossomo (binário) para inteiro
int chromosomeToInt(const std::vector<int>& chromosome) {
    int x = 0;
    for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
        x += chromosome[i] << (CHROMOSOME_SIZE - 1 - i);
    }
    return x;
}

// Gerar população inicial
std::vector<std::vector<int>> initializePopulation() {
    std::vector<std::vector<int>> population(POPULATION_SIZE, std::vector<int>(CHROMOSOME_SIZE));
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < CHROMOSOME_SIZE; ++j) {
            population[i][j] = rand() % 2;
        }
    }
    return population;
}

// Seleção por torneio
std::vector<int> tournamentSelection(const std::vector<std::vector<int>>& population, const std::vector<int>& fitnessValues) {
    std::vector<int> selected;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        int best = rand() % POPULATION_SIZE;
        for (int j = 0; j < 3; ++j) { // Torneio de tamanho 3
            int contender = rand() % POPULATION_SIZE;
            if (fitnessValues[contender] > fitnessValues[best]) {
                best = contender;
            }
        }
        selected.push_back(best);
    }
    return selected;
}

// Cruzamento (crossover) de um ponto
void crossover(std::vector<int>& parent1, std::vector<int>& parent2) {
    if (randomDouble() < CROSSOVER_RATE) {
        int point = rand() % CHROMOSOME_SIZE;
        for (int i = point; i < CHROMOSOME_SIZE; ++i) {
            std::swap(parent1[i], parent2[i]);
        }
    }
}

// Mutação
void mutate(std::vector<int>& chromosome) {
    for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
        if (randomDouble() < MUTATION_RATE) {
            chromosome[i] = 1 - chromosome[i];
        }
    }
}

int main() {
    srand(time(0));

    // Inicializar população
    auto population = initializePopulation();

    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        // Avaliar aptidão
        std::vector<int> fitnessValues(POPULATION_SIZE);
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            int x = chromosomeToInt(population[i]);
            fitnessValues[i] = fitness(x);
        }

        // Seleção
        auto selected = tournamentSelection(population, fitnessValues);

        // Cruzamento e mutação
        std::vector<std::vector<int>> newPopulation;
        for (int i = 0; i < POPULATION_SIZE; i += 2) {
            auto parent1 = population[selected[i]];
            auto parent2 = population[selected[i + 1]];
            crossover(parent1, parent2);
            mutate(parent1);
            mutate(parent2);
            newPopulation.push_back(parent1);
            newPopulation.push_back(parent2);
        }

        // Substituir população
        population = newPopulation;

        // Encontrar a melhor solução
        int bestFitness = *std::max_element(fitnessValues.begin(), fitnessValues.end());
        std::cout << "Geração " << generation << ": Melhor aptidão = " << bestFitness << std::endl;
    }

    return 0;
}