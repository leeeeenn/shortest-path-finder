/*leen ahmad
1212028
sec4*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CITYNAME 100
#define MAXVAL 100000000
struct Node
{
    int id;
    struct Edge* edges;
};

struct Edge
{
    struct Node* destination;
    int weight;
    struct Edge* next;
};

struct Node* createNode(int id)
{
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->id = id;
    newNode->edges = NULL;
    return newNode;
}
int select_min(int val[], int processed[]);
void dijkstra(struct Node* graph[15], char source [], char destination [], int numNodes, char** citynames, FILE * outputt);
void add_Edge(struct Node* source, struct Node* destination, int weight);
void printGraph(struct Node** graph, char** cityNames, int numNodes);
int get_dest(struct Node** graph, int* numNodes, char** cityNames, char* cityName);
void BFS(struct Node** graph, int source, int destination, int num_Cities, char** city_Names, FILE* outputt);

int main()
{
    FILE * outputt;
    FILE* file = fopen("cities.txt", "r");
    if (file == NULL)
    {
        printf("can't open the file :(.\n");
        return 1;
    }

    struct Node* graph[15]; // 15 cities in the file
    char* cityNames[15];
    int num_nodes = 0;

    char source_city[50] = {'\0'};
    char dest_city[50] = {'\0'};
    int weight;

    while (fscanf(file, "%s %s %dkm\n", source_city, dest_city, &weight) != EOF)
    {
        // Get the specific distances for source and destination nodes
        int source_val = get_dest(graph, &num_nodes, cityNames, source_city);
        int destination_val = get_dest(graph, &num_nodes, cityNames, dest_city);

        // Add an edge from source to destination
        add_Edge(graph[source_val], graph[destination_val], weight);
    }
    int choice;
    printf("welcome !\n please enter a choice from the menu: \n");
    printf(" 1. load cities and construct graph \n 2. enter a source city \n 3. enter e destination to calcluate the BFS and dijkstra specifically with the source \n 4. exit");
    scanf("%d",&choice);
    while (choice!= 4)
    {
        switch (choice)
        {
        case 1 :
        {
            printGraph(graph, cityNames, num_nodes);
            break;
        }

        case 2:
        {

            printf("please enter a source city :");
            scanf("%s", source_city);
            break;

        }
        case 3:
        {
            printf("the source name is %s \n", &source_city);
            printf("please enter a destination city");
            scanf("%s", dest_city);
            int source_val = get_dest(graph, &num_nodes, cityNames, source_city);
        int destination_val = get_dest(graph, &num_nodes, cityNames, dest_city);
            BFS(graph, source_val, destination_val, num_nodes, cityNames, outputt);
            dijkstra(graph, source_city, dest_city, num_nodes, cityNames, outputt);
            break;
        }
        }
        printf("please enter a choice from the menu: \n");
        printf(" 1. load cities and construct graph \n 2. enter a source city to calculate distances between all cities \n 3. enter e destination to calcluate the BFS and dijkstra specifically \n 4. exit");
        scanf("%d",&choice);

    }
    if (choice ==4 )
    {
        printf("thank you for using the program!");
         outputt = fopen("shortest_distance", "w");
        if(outputt == NULL){
            printf("couldn't open the file :( \n ");
            return 1;
        }
        //dijkstra(graph, source_city, dest_city, num_nodes, cityNames, 4, outputt);
        //BFS(graph, source_val, destination_val, num_nodes, cityNames,4, outputt);
        exit(0);
    }
    fclose(file);
    fclose(outputt);


    // Free memory
    for (int i = 0; i < num_nodes; i++)
    {
        struct Edge* edge = graph[i]->edges;
        while (edge != NULL)
        {
            struct Edge* nextEdge = edge->next;
            free(edge);
            edge = nextEdge;
        }
        free(graph[i]);
        free(cityNames[i]);
    }

    return 0;
}
void add_Edge(struct Node* source, struct Node* destination, int weight)
{
    struct Edge* newEdge = (struct Edge*)malloc(sizeof(struct Edge));
    newEdge->destination = destination;
    newEdge->weight = weight;
    newEdge->next = source->edges;
    source->edges = newEdge;
}
void printGraph(struct Node** graph, char** cityNames, int numNodes)
{
    for (int i = 0; i < numNodes; i++)
    {
        struct Node* node = graph[i];
        printf("%s (%d) -> ", cityNames[node->id], node->id);
        struct Edge* edge = node->edges;
        while (edge != NULL)
        {
            printf("%s (%dkm) ", cityNames[edge->destination->id], edge->weight);
            edge = edge->next;
        }
        printf("\n");
    }
}

int get_dest(struct Node** graph, int* numNodes, char** cityNames, char* cityName)
{
    for (int i = 0; i < *numNodes; i++)
    {
        if (strcmp(cityName, cityNames[i]) == 0)
        {
            return graph[i]->id;
        }
    }
    //this part is to create the city's node if it isn't in the graph
    struct Node* newNode = createNode(*numNodes);
    graph[*numNodes] = newNode;
    cityNames[*numNodes] = strdup(cityName);
    (*numNodes)++;
    return *numNodes - 1;
}
/*
int select_min(int val [], int processed [] )
{

    int minimum_val = MAXVAL;
    int vertex;
    for(int i =0; i<15; ++i)
    {
        if(processed[i]== -1 && val[i]<minimum_val )
        {
            vertex =i;
            minimum_val = val[i];

        }

    }
    return vertex;

}*/

//the dijkstra function
void dijkstra(struct Node* graph[15], char source[], char destination[], int numNodes, char** citynames, FILE * outputt)
{
    int source_ID = get_dest(graph, &numNodes, citynames, source);
    int destination_ID = get_dest(graph, &numNodes, citynames, destination);

    int value[15];
    int processed[15];
    int parent[15];

    for (int i = 0; i < numNodes; i++)
    {
        value[i] = MAXVAL; //all nodes should start with the max value
        processed[i] = 0;   // none of them should be visited before
        parent[i] = -1;     //they initially have no parent
    }

    value[source_ID] = 0;    // the parent's "source" value should be zero

    for (int count = 0; count < 14; count++)
    {
        int u = -1; //to find the number of nodes visited
        int min_value = MAXVAL; //so it would reduce

        // Find the vertex with the minimum value
        for (int v = 0; v < 15; v++)
        {
            if (processed[v] == 0 && value[v] < min_value) //the node shouldn't be processed before and the value must be less because it's shortest path
            {
                min_value = value[v];
                u = v;
            }
        }

        if (u == -1)  //all nodes were visited
        {
            break;
        }

        processed[u] = 1;   // so we can know which nodes are left to visit

        struct Edge* edge = graph[u]->edges;
        while (edge != NULL) //after visiting u must visit the edges connected with it
        {
            int v = edge->destination->id;
            int weight = edge->weight;

            if (processed[v] == 0 && value[u] != MAXVAL && value[u] + weight < value[v])
            {
                value[v] = value[u] + weight;
                parent[v] = u;
            }

            edge = edge->next;
        }
    }


    if (value[destination_ID] == MAXVAL)
    {

           fprintf( outputt,"failed to find a path %s to %s.\n", source, destination);

        printf("failed to find a path %s to %s.\n", source, destination);

    }else
    { {
 fprintf(outputt, "Shortest path using dijkstra from %s to %s is as follow:\n", source, destination);

        printf("Shortest path using dijkstra from %s to %s is as follow:\n", source, destination);

        // Print the path
        int cities_otw[15];
        int total_length = 0;
        int node = destination_ID;

        while (node != -1)
        {
            cities_otw[total_length++] = node;
            node = parent[node];
        }

        for (int i = total_length - 1; i >= 0; i--)
        {
        fprintf(outputt,"%s", citynames[cities_otw[i]]);

            printf("%s", citynames[cities_otw[i]]);
            if (i != 0)
            {
            fprintf(outputt, " -> ");

                printf(" -> ");
            }
        }

             fprintf(outputt,"\nDistance (Dijkstra): %d km\n", value[destination_ID]);


        printf("\nDistance (Dijkstra): %d km\n", value[destination_ID]);
    }

}}
void BFS(struct Node** graph, int source, int destination, int num_Cities, char** city_Names, FILE * outputt)
{
    int processed[num_Cities];
    int prev[num_Cities];
    int values[num_Cities];

    for (int i = 0; i < num_Cities; i++)//set the initial values
    {
        processed[i] = 0;
        prev[i] = -1;
        values[i] = -1;
    }
    processed[source] = 1;
    values[source] = 0;
    //creating a queue
    int queue[num_Cities];
    int front = 0;
    int rear = 0;
    queue[rear++] = source;//enquque the source
    while(front != rear )
    {
        int current_city =queue[front++];
        struct Node* city = graph[current_city];
        struct Edge* edge = city -> edges;

        while(edge!=NULL)
        {
            int follow = edge ->destination->id;
            if(processed[follow] ==0)
            {
                processed[follow] = 1;
                prev[follow] = current_city;
                values[follow] = values[current_city] + edge -> weight;
                queue[rear++] = follow;



               if(follow== destination)
                    break; //early stop
            }
            edge = edge ->next;
        }


        if (rear != front && queue[front] == destination)
            break;  //early stop
    }

    //print the shortest path and its distance
    int cities_otw[num_Cities];
    int cities_num = 0;//number of edges
    int current = destination;//going backward from the destination using previous array

    while (current != -1)
    {
        cities_otw[cities_num++] = current;
        current = prev[current];
    }
  fprintf(outputt, "BFS path is as follow  : ");

    printf("BFS path is as follow  : ");
    for (int i = cities_num - 1; i >= 0; i--)
    {
        fprintf(outputt, "%s ", city_Names[cities_otw[i]]);

        printf("%s ", city_Names[cities_otw[i]]);
        if (i != 0)
            printf(" -> ");
    }

        fprintf(outputt,"\n distance (BFS): %dkm\n", values[destination]);


    printf("\n distance (BFS): %dkm\n", values[destination]);


}
