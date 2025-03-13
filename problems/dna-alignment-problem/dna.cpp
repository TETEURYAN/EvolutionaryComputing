#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

// Constantes
const int POPULATION_SIZE = 10;
const int CHROMOSOME_SIZE = 5;
const int MAX_GENERATIONS = 100;
const double CROSSOVER_RATE = 0.7;
const double MUTATION_RATE = 0.01;
const int DEFICIENT_ZERO_THRESHOLD = 3;
const int PENALTY = -100;

// Função de aptidão
int fitness(const std::vector<int>& chromosome) {
    int x = 0;
    int zeros = std::count(chromosome.begin(), chromosome.end(), 0);

    // Converter cromossomo para inteiro
    for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
        x += chromosome[i] << (CHROMOSOME_SIZE - 1 - i);
    }

    // Aplicar penalidade se o número de zeros exceder o limite
    if (zeros > DEFICIENT_ZERO_THRESHOLD) {
        return x * x + PENALTY;
    }
    return x * x;
}

// Gerar população inicial
std::vector<std::vector<int>> initializePopulation() {
    std::vector<std::vector<int>> population(POPULATION_SIZE, std::vector<int>(CHROMOSOME_SIZE));
    std::srand(std::time(0));
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        for (int j = 0; j < CHROMOSOME_SIZE; ++j) {
            population[i][j] = std::rand() % 2;
        }
    }
    return population;
}

// Seleção por torneio
std::vector<int> tournamentSelection(const std::vector<std::vector<int>>& population, const std::vector<int>& fitnessValues) {
    std::vector<int> selected;
    for (int i = 0; i < POPULATION_SIZE; ++i) {
        int best = std::rand() % POPULATION_SIZE;
        for (int j = 0; j < 3; ++j) { // Torneio de tamanho 3
            int contender = std::rand() % POPULATION_SIZE;
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
    if ((double)std::rand() / RAND_MAX < CROSSOVER_RATE) {
        int point = std::rand() % CHROMOSOME_SIZE;
        for (int i = point; i < CHROMOSOME_SIZE; ++i) {
            std::swap(parent1[i], parent2[i]);
        }
    }
}

// Mutação
void mutate(std::vector<int>& chromosome) {
    for (int i = 0; i < CHROMOSOME_SIZE; ++i) {
        if ((double)std::rand() / RAND_MAX < MUTATION_RATE) {
            chromosome[i] = 1 - chromosome[i];
        }
    }
}

int main() {
    std::srand(std::time(0));

    // Inicializar população
    auto population = initializePopulation();

    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        // Avaliar aptidão
        std::vector<int> fitnessValues(POPULATION_SIZE);
        for (int i = 0; i < POPULATION_SIZE; ++i) {
            fitnessValues[i] = fitness(population[i]);
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