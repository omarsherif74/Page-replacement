#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void FIFO(int pgfrmcount,int stream[],int n)
{
    int queue[pgfrmcount]; //queue to store pages in memory after every page reference
    int qcount=0; //number of elements in queue
    int stream_count=n-1; //number of pages references
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int p=0; //present bit (p=1 if in MM, otherwise p=0)
    int index=0; //index of page to replace
    int pg_fault=0;
    int j=0;
    while(j<stream_count)
    {
        for(int i=0; i<qcount; i++)
        {
            if(stream[j]!=queue[i]) //if page not in memory
            {
                p=0;
            }
            else //page resident in memory
            {
                p=1;
                break;
            }
        }


        if(p==1)  //if page already found in memory
        {
            printf("%02d     ",stream[j]);

        }
        else  //if page not in memory
        {

            if(p==0 && qcount<pgfrmcount) //if page not in memory but empty frame exists
            {
                queue[qcount]=stream[j]; //enqueue page in FIFO queue
                ++qcount;
                printf("%02d     ",stream[j]);
            }

            else if(p==0 && qcount==pgfrmcount) //if page not in memory and no empty frame (page fault)  ==> replacement
            {
                printf("%02d F   ",stream[j]);
                ++pg_fault;
                queue[index++]=stream[j]; //replace page at index with current page
                if(index==pgfrmcount)
                {
                    index=0; //reset index to start from first page in memory
                }
            }
        }

        for(int k=0; k<qcount; k++) //print queue elements after every page reference
        {
            printf("%02d ",queue[k]);
        }

        printf("\n");
        j++;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",pg_fault);
}


void LRU(int pgfrmcount,int stream[],int n)
{
    int stream_count=n-1;
    int time[pgfrmcount]; //array to tell when page was last accessed
    int p=0;
    int pg_fault=0;
    int queue[pgfrmcount];
    int qcount=0;
    for(int i=0; i<pgfrmcount; i++)
    {
        time[i]=0; //initialize all pages to zero
    }

    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int j=0;
    while(j<stream_count)
    {
        for(int i=0; i<qcount; i++)
        {
            if(stream[j]!=queue[i]) //if page not in memory
            {
                p=0;
            }
            else //if page in memory
            {
                p=1;
                break;
            }
        }


        if(p==1) //if page already found in memory
        {
            printf("%02d     ",stream[j]);
        }
        else //if page not in memory
        {
            if(qcount<pgfrmcount) //if page not in memory but empty frame exists
            {
                queue[qcount]=stream[j]; //enqueue FIFO queue
                ++qcount;

                printf("%02d     ",stream[j]);
            }
            else if(qcount==pgfrmcount) //if page not in memory and no empty frame (page fault) ==> replacement
            {
                printf("%02d F   ",stream[j]);
                ++pg_fault; //page fault (miss)


                for(int q=0; q<qcount; q++)//loop over all pages in queue
                {
                    for(int z=j-1; z>=0; z--)//loop over all pages in stream (start from most recently accessed page)
                    {
                        if(queue[q]==stream[z])
                        {
                            time[q]=z; //save index when page was last accessed
                            break;
                        }
                    }
                }
                int min_index=getindexMin(time,pgfrmcount);//get index of page with min. entry in time array least recently used)
                queue[min_index]=stream[j];//replace this page with current page
            }
        }

        for(int k=0; k<qcount; k++)
        {
            printf("%02d ",queue[k]);
        }

        printf("\n");
        j++;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",pg_fault);
}


void optimal(int pgfrmcount,int stream[],int n)
{
    int stream_count=n-1;
    int time[stream_count];//array to store pages to be accessed in the future after current page
    int p=0;
    int pg_fault=0;
    int queue[pgfrmcount];
    int qcount=0;
    int future_index[stream_count];//array to store, for every page in queue, the number of pages between current access and first future access
    for(int i=0; i<stream_count; i++)
    {
        future_index[i]=0;
        time[i]=0;
    }

    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int j=0;
    while(j<stream_count)
    {
        for(int i=0; i<qcount; i++)
        {
            if(stream[j]!=queue[i]) //if page not in memory
            {
                p=0;
            }
            else //if page in memory
            {
                p=1;
                break;
            }
        }


        if(p==1) //if page already found in memory
        {
            printf("%02d     ",stream[j]);
        }
        else //if page not in memory
        {
            if(qcount<pgfrmcount) //if page not in memory but empty frame exists
            {
                queue[qcount]=stream[j]; //enqueue FIFO queue
                ++qcount;

                printf("%02d     ",stream[j]);
            }
            else if(qcount==pgfrmcount) //if page not in memory and no empty frame (page fault) ==> replacement
            {
                printf("%02d F   ",stream[j]);
                ++pg_fault; //page fault (miss)
                int x=j+1;//to start from next page reference
                for(int z=0; z<stream_count-j-1; z++) //  remaining stream of pages to be checked
                {
                    time[z]=stream[x++];
                }

                for(int i=0; i<stream_count; i++)
                {
                    future_index[i]=0;//reset before incrementing again
                }

                for(int z=0; z<qcount; z++)
                {
                    int distance=1;
                    for(int s=0; s<stream_count-j-1; s++)
                    {
                        if(queue[z]==time[s])
                        {
                            future_index[z]+=distance;
                            break;
                        }
                        else if(s==stream_count-j-2) //page not referenced again
                        {
                            future_index[z]=50000;
                        }
                        distance++;
                    }
                }

                int max_index=getindexMax(future_index,stream_count);//get index of farthest page
                queue[max_index]=stream[j];//replace

            }
        }

        for(int k=0; k<qcount; k++)
        {
            printf("%02d ",queue[k]);
        }

        printf("\n");
        j++;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",pg_fault);
}


void clock(int pgfrmcount,int stream[],int n)
{
    int stream_count=n-1;
    int use[pgfrmcount];//array to store use bits of every page in queue

    for(int z=0; z<pgfrmcount; z++)
    {
        use[z]=0; //initialize all use bits of all pages to 0
    }

    int p=0;
    int pg_fault=0;
    int queue[pgfrmcount];
    int qcount=0;
    int ptr=0;//next frame pointer to tell which page's use bit must be checked now
    int index;//stores index of page already in memory if accessed again
    printf("Page   Content of Frames\n");
    printf("----   -----------------\n");
    int j=0;
    while(j<stream_count)
    {
        for(int i=0; i<qcount; i++)
        {
            if(stream[j]!=queue[i]) //if page not in memory
            {
                p=0;
            }
            else //if page in memory
            {
                p=1;
                index=i;//store index at which page resides in queue
                break;
            }
        }


        if(p==1) //if page already found in memory
        {
            printf("%02d     ",stream[j]);
            use[index]=1;//set use bit to 1
        }

        else //if page not in memory
        {
            if(qcount<pgfrmcount) //if page not in memory but empty frame exists
            {
                queue[qcount]=stream[j]; //enqueue FIFO queue
                use[qcount]=1;//set use bit to 1

                ++qcount;

                printf("%02d     ",stream[j]);
            }
            else if(qcount==pgfrmcount) //if page not in memory and no empty frame (page fault) ==> replacement
            {
                printf("%02d F   ",stream[j]);
                ++pg_fault; //page fault (miss)


                while(use[ptr]!=0)
                {
                    use[ptr]=0;
                    ptr++;//move pointer to next page
                    if(ptr==pgfrmcount)
                    {
                        ptr=0;//reset ptr
                    }
                }

                if(use[ptr]==0)
                {
                    //replace
                    queue[ptr]=stream[j];

                    use[ptr]=1;//set use bit = 1
                    ptr++;//move pointer to next page
                    if(ptr==pgfrmcount)
                    {
                        ptr=0;//reset ptr
                    }
                }

            }
        }

        for(int k=0; k<qcount; k++)
        {
            printf("%02d ",queue[k]);
        }

        printf("\n");
        j++;
    }
    printf("-------------------------------------\n");
    printf("Number of page faults = %d\n",pg_fault);
}


int getindexMax(int arr[],int size)
{
    int max_index=0;
    for(int i=1; i<size; i++)
    {
        if(arr[i]>arr[max_index])
        {
            max_index=i;
        }
    }
    return max_index;
}

int getindexMin(int arr[],int size)
{
    int min_index=0;
    for(int i=1; i<size; i++)
    {
        if(arr[i]<arr[min_index])
        {
            min_index=i;
        }
    }
    return min_index;
}


int main()
{
    char algorithm[20];
    int pageFrameCount;
    int stream[5000];
    int stream_count=0;
    scanf("%d",&pageFrameCount);
    scanf("%s",algorithm);
    printf("Replacement Policy = %s\n",algorithm);
    printf("-------------------------------------\n");

    do
    {
        scanf("%d",&stream[stream_count]);
    }
    while(stream[stream_count++]!=-1);

    if(strcmp(algorithm,"FIFO")==0)
    {
        FIFO(pageFrameCount,stream,stream_count);
    }
    else if(strcmp(algorithm,"LRU")==0)
    {
        LRU(pageFrameCount,stream,stream_count);
    }
    else if(strcmp(algorithm,"OPTIMAL")==0)
    {
        optimal(pageFrameCount,stream,stream_count);
    }
    else if(strcmp(algorithm,"CLOCK")==0)
    {
        clock(pageFrameCount,stream,stream_count);
    }

    return 0;
}
