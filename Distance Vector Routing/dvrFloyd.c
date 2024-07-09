#include <stdio.h>

#define MAX_NODES 20
#define INFINITY 999

int main(void)
{
    int numNodes;

    // Define a struct for routers
    struct router
    {
        int distance[MAX_NODES];
        int adjacent[MAX_NODES];
    } routers[MAX_NODES];

    printf("\nEnter the number of nodes: ");
    scanf("%d", &numNodes);

    printf("\nEnter %d if nodes don't have a path between them\nSelf-distance will be overwritten as 0", INFINITY);

    // Initialize distances and adjacencies
    for (int i = 0; i < numNodes; i++)
    {
        for (int j = 0; j < numNodes; j++)
        {
            printf("\nEnter the distance between node[%d] and node[%d]: ", i + 1, j + 1);
            scanf("%d", &routers[i].distance[j]);
            routers[i].adjacent[j] = j;

            // Self-distance should be 0
            if (i == j)
            {
                routers[i].distance[j] = 0;
            }
        }
    }

    // Compute shortest paths
    for (int i = 0; i < numNodes; i++) // ieterate through all routers
    {
        for (int j = 0; j < numNodes; j++) // through all Destination Router
        {
            for (int k = 0; k < numNodes; k++) // each possible intermediate router
            {
                if (routers[i].distance[j] > routers[i].distance[k] + routers[k].distance[j])
                {
                    routers[i].distance[j] = routers[i].distance[k] + routers[k].distance[j];
                    routers[i].adjacent[j] = k;
                }
            }
        }
    }

    // Display routing tables
    for (int i = 0; i < numNodes; i++)
    {
        printf("\n\nRouter[%d] Table", i + 1);
        for (int j = 0; j < numNodes; j++)
        {
            printf("\n\tDistance to router[%d] through router[%d] is [%d]", j + 1, routers[i].adjacent[j] + 1, routers[i].distance[j]);
        }
    }

    return 0;
}
