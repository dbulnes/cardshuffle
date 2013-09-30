#include <stdio.h>
#include <malloc.h>
#include <conio.h>

////////////////////////////////////////////////////////////Structure definition representing each card in the deck
//////////////////////////////////////////////////////////

struct card
{
    int value;
    struct card * next;
};

//////////////////////////////////////////////////////////////////////////
//Global Declarations
//////////////////////////////////////////////////////////////////////////
//On=1 or Off=0 the ability to print out detailed report on each step
int PRINT_EACH_STEP = 0;

//Number of cards: Taken as input from the user.
int iNumberOfCards = 0;

//Head and Tail nodes of cards in hand
struct card *head_on_hand = NULL;
struct card *tail_on_hand = NULL;

//Head and Tail nodes of cards on table
struct card *head_on_table = NULL;
struct card *tail_on_table = NULL;

//////////////////////////////////////////////////////////////////////////
//Function Name: initialize_headtail
//
//Purpose: To initialize the head and tail nodes of both hand and table lists
//
//Parameters: NONE
//
//Returns: NOTHING
//////////////////////////////////////////////////////////////////////////
void initialize_headtail()
{
    head_on_table = (struct card*)malloc(sizeof(struct card));
    head_on_hand = (struct card*)malloc(sizeof(struct card));
    tail_on_table = (struct card*)malloc(sizeof(struct card));
    tail_on_hand = (struct card*)malloc(sizeof(struct card));
    
    head_on_hand->value = -1;
    head_on_table->value = -1;
    tail_on_hand->value = -1;
    tail_on_table->value = -1;
    
    head_on_table->next = tail_on_table;
    head_on_hand->next = tail_on_hand;
    tail_on_table->next = NULL;
    tail_on_hand->next = NULL;
}

//////////////////////////////////////////////////////////////////////////
//Function Name: initialize
//
//Purpose: To initialize the card deck.
//
//Parameters: NONE
//
//Returns: NOTHING
//////////////////////////////////////////////////////////////////////////
void initialize()
{
    int iCount;
    struct card* newone = NULL;
    initialize_headtail();
    //for the number of cards, as per user input
    for(iCount = iNumberOfCards; iCount>0; iCount--)
    {
        //create a card and insert it into the deck
        newone = (struct card*)malloc(sizeof(struct card));
        newone->value = iCount;
        newone->next = head_on_hand->next;
        head_on_hand->next = newone;
        newone = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////
//Function Name: print_hand
//
//Purpose: To print the details of cards in hand.
//
//Parameters: NONE
//
//Returns: NOTHING
//////////////////////////////////////////////////////////////////////////
void print_hand()
{
    if(PRINT_EACH_STEP == 1)
    {
        struct card *temp;
        //print the nodes from head_on_hand to tail_on_hand
        temp = head_on_hand;
        printf("HAND->");
        while(temp->next!=tail_on_hand)
        {
            printf("%d->",temp->next->value);
            temp = temp->next;
        }
        printf("TAIL\n");
    }
}

//////////////////////////////////////////////////////////////////////////
//Function Name: print_table
//
//Purpose: To print the details of cards on table.
//
//Parameters: NONE
//
//Returns: NOTHING
//////////////////////////////////////////////////////////////////////////
void print_table()
{
    if(PRINT_EACH_STEP == 1)
    {
        struct card *temp;
        //print the nodes from head_on_table to tail_on_table
        temp = head_on_table;
        printf("TABLE->");
        while(temp->next!=tail_on_table)
        {
            printf("%d->",temp->next->value);
            temp = temp->next;
        }
        printf("TAIL\n");
    }
}

//////////////////////////////////////////////////////////////////////////
//Function Name: check_for_initial
//
//Purpose: To check whether the order of cards is same as the initial order.
//
//Parameters: NONE
//
//Returns: 1 - Success
//	 0 - Failure
//////////////////////////////////////////////////////////////////////////
int check_for_initial()
{
    struct card *temp;
    int iCheck = 1;
    temp = head_on_hand;
    while(temp->next!=tail_on_hand)
    {
        if(temp->next->value == iCheck)
        {
            iCheck++;
            temp = temp->next;
            continue;
        }
        else
            return 0;
    }
    return 1;
}

//////////////////////////////////////////////////////////////////////////
//Function Name: check_for_initial
//
//Purpose: To shuffle the card deck once.
//	 After shuffle the cards will be placed on the table.
//
//
//Method used: Always action is performed on the top most card
//	 1. Put the card on the table.
//	 2. Put the card at the end(in hand).
//	 3. Repeat 1 and 2 until there are cards left in hand
// (Comments in the function body will use these step numbers for reference)
//
//Parameters: NONE
//
//Returns: NOTHING
//////////////////////////////////////////////////////////////////////////
void shuffle_one_round()
{
    struct card *temp_hand = head_on_hand;
    struct card *temp = NULL;
    head_on_table->next = tail_on_table;
    
    //This while loop will provide a pointer to last card
    //This will be needed to place a card at the end.
    while(temp_hand->next!=tail_on_hand)
    {
        temp_hand = temp_hand->next;
    }
    
    //Repeat until there are cards left in hand
    while(head_on_hand->next!=tail_on_hand)
    {
        /* Once you place the first card on the table,
         It should be inserted at head of table list.
         And removed from the hand list.
         Thus head of the hand list will point to
         the second node after first one is removed
         */
        //Pointer to the second one.
        temp = head_on_hand->next->next;
        
        //Insert the first card at head of table list
        head_on_hand->next->next = head_on_table->next;
        
        //Remove the first card from hand list - these 3 statements will do that
        head_on_table->next = head_on_hand->next;
        head_on_hand->next = temp;
        temp = temp->next;
        
        /* Now we have to place the next card at the end of hand list
         Remember we already have the pointer to the last card in temp_hand
         */
        //if there are cards left
        if(head_on_hand->next!=tail_on_hand)
        {
            //Get the card.
            temp_hand->next = head_on_hand->next;
            //remove it from the first position.
            head_on_hand->next = head_on_hand->next->next;
            //insert it at the end.
            temp_hand = temp_hand->next;
            //Make it the last card.
            temp_hand->next = tail_on_hand;
        }
    }
    
}

//////////////////////////////////////////////////////////////////////////
//Function Name: take_in_hand
//
//Purpose: Once shuffled card will be on table.
//	 If the order is not same, then call this function,
//	 to take it back to your hand
//
//Parameters: NONE
//
//Returns: NOTHING
//////////////////////////////////////////////////////////////////////////
void take_in_hand()
{
    struct card *temp = head_on_table;
    //make the head node of table as head node of hand.
    head_on_hand->next = head_on_table->next;
    //loop to find the last card
    while(temp->next!=tail_on_table)
    {
        temp=temp->next;
    }
    //make the last card point to the tail node of hand
    temp->next = tail_on_hand;
    //make head in table point to tail. thus table list is now empty
    head_on_table->next = tail_on_table;
}

//////////////////////////////////////////////////////////////////////////
//Function Name: main
//
//Purpose: Main function
//	 Gets the user input and calls the functions needed, to provide
//	 the user output.
//////////////////////////////////////////////////////////////////////////
int main()
{
    long Count = 0;
    
    int i;
    
    //Get the number of cards, Ask the user again on error
    while(1)
    {
        printf("Enter the number of cards in the deck: ");
        scanf("%d",&iNumberOfCards);
        
        if(iNumberOfCards<1)
        {
            printf("Deck should contain one or more cards\n");
            continue;
        }
        break;
    }
    
    //Give the user option to look into each step in the process.
    //If not interested he can just look into the final answer
    while(1)
    {
        printf("Do you want to print each step? Enter 1 for Yes and 0 for No: ");
        scanf("%d",&PRINT_EACH_STEP);
        
        if(PRINT_EACH_STEP != 0 && PRINT_EACH_STEP != 1)
        {
            printf("Please enter the correct choice (1 or 0)\n");
            continue;
        } 
        break; 
    } 
    //313, 367 
    
    for(i = 1; i < 313; i++) 
    { 
        iNumberOfCards = i; 
        //Once the user input is in, initialize the deck. 
        initialize(); 
        
        //Shuffle it until you get back the initial order 
        while(1) 
        { 
            if(PRINT_EACH_STEP)printf("\nBEFORE SHUFFLE\n"); 
            print_hand(); 
            print_table(); 
            
            //Shuffle 
            shuffle_one_round(); 
            if(PRINT_EACH_STEP)printf("\nAFTER SHUFFLE\n"); 
            print_hand(); 
            print_table(); 
            Count++; 
            
            //take it back to your hand 
            take_in_hand(); 
            
            //See if the order is same as the initial order 
            if(check_for_initial()) 
            { 
                //if yes, print the output and exit 
                if(PRINT_EACH_STEP)printf("\nThe order is same as initial order.\n\n"); 
                printf("Number of Rounds = %d\n", Count); 
                break; 
            } 
            //if not same then continue to shuffle again 
            if(PRINT_EACH_STEP)printf("\nThe order is not same as initial order.\n TAKE IT AND SHUFFLE AGAIN\n"); 
        } 
        
        Count = 0; 
    } 
    getch(); 
} 


////////////////////////////////////////////////////////////////////////// 
//END OF FILE 
//////////////////////////////////////////////////////////////////////////