
#include <stdio.h>  //printf,fgets
#include <string.h> //strcpy
#include <stdlib.h> //atoi

struct Menu
{
    int     	nCount;
    char    	itemName[64];
    double   	price;
};

/*
//Example usage of structure
struct Menu oneItem;

oneItem.nCount = 10; //use dot operator to access members within structure
//oneItem.item = "Dosa"; //string cannot be assigned like this
strcpy(oneItem.item, "Dosa"); //use strcpy to copy string, string.h

oneItem.price = 60.50;

*/

struct Menu availableItems[] = { {0, "Dosa", 60.50},
                                {0, "Idli", 50.0},
                                {0, "Poori", 80.0},
                                {0, "Ghee Roast", 120.0},
                                {0, "Masala Roast", 110.0},
                                {0, "Chapathi", 70.0},
                                {0, "Idiyappam", 60.0},
                                {0, "Pongal", 100.0},
                                {0, "Vada", 80.0},
                                {0, "Coffee", 60.0},
                                {0, "Tea", 60.0},
                                {0, "Horlicks", 80.0}
};


#define	HYPHEN_COUNT				96		
#define MAX_ORDERABLE_ITEMS         100 //this is the capacity
#define MAX_HOW_MANY_PER_CHOICE     30

int orderedIndex = -1; //how many items are actually there in the order
struct Menu orderedItems[MAX_ORDERABLE_ITEMS]; //this is where all the orders will get added

int getAvailableItemsCount() 
{
    return sizeof(availableItems) / sizeof(struct Menu);
}

double getIgstPercent(double price)
{
    return price * 0.12; //this is percentage
}

double getCgstPercent(double price)
{
    return price * 0.12; //this is percentage
}

double getGstPrice(double price)
{
    return price + getIgstPercent(price) + getCgstPercent(price);
}

void printHyphenLine()
{
	for(int i = 0; i < HYPHEN_COUNT; i++)
	{
		printf("-");
	}

	printf("\n");
}

void displayMenu()
{
    int size = getAvailableItemsCount();
    
	printHyphenLine();
	printf("%48s","Hotel Menucard\n");
	printHyphenLine();

	printf("%-10s\t%-30s\t%s\n", "ItemNumber", "ItemName", "ItemPrice");

	printHyphenLine();

    for(int index = 0; index < size; index++)
    {
        printf("%-10d\t%-30s\t%0.2f\n", (index + 1), availableItems[index].itemName, 
availableItems[index].price);
    }

	printHyphenLine();
}

void addOrder(int nChoice, int nCount)
{

    if((orderedIndex + 1)  >= 0 && (orderedIndex + 1) < MAX_ORDERABLE_ITEMS)
    {
        orderedIndex++; //this increases size of the ordered items

        orderedItems[orderedIndex].nCount = nCount;
        strcpy(orderedItems[orderedIndex].itemName, availableItems[nChoice].itemName); //lets say nChoice is 1, here the value should be Idli
        orderedItems[orderedIndex].price = availableItems[nChoice].price;
    }
}

void printOrderSummary()
{
    int nItemsTotal = 0;
    double gstTotal = 0.0;
    double nTotalPrice = 0.0;
    
    printf("\n");
    printf("Here is Summary of Your Order\n");

	printHyphenLine();
    printf("%-30s\t\t%-8s\t%-8s\t\t%s\n", "Item", "Count", "GST Price", "Total");
    printHyphenLine();

    for(int index = 0; index <= orderedIndex; index++)
    {
        nItemsTotal +=  orderedItems[index].nCount;
        double currentGSTPrice = getGstPrice(orderedItems[index].price);
        double nGstPrice = currentGSTPrice * orderedItems[index].nCount;
        gstTotal += currentGSTPrice;
        nTotalPrice += nGstPrice;
        printf("%-30s\t\t%-8d\t%0.2f Rs\t\t%0.2f Rs\n", orderedItems[index].itemName, orderedItems[index].nCount, 
currentGSTPrice, nGstPrice);
    }
    printHyphenLine();
    printf("\n%-30s\t\t\t\t\t\t\t%0.2f Rs\n", "Grand Total", nTotalPrice);
	printHyphenLine();
	printf("\n");

}

int takeOrder()
{
    printf("\nYour Choice:");
    
	int nChoice = -1;

    char sChoice[128];
    fgets(sChoice, 128, stdin); //we use fgets to discard \n, scanf is buggy with \n, so we wont use scanf
   
   	//humans use natural number or 1 based 
    nChoice = atoi(sChoice); //alphabet to integer
    
    nChoice--; //convert to whole number or zero based

    
    if(nChoice >= 0 && nChoice < getAvailableItemsCount())
    {
        int nHowMany = 0;
        printf("\nHow Many:");
        
        char sHowMany[128];
        fgets(sHowMany, 128, stdin);
        
        nHowMany = atoi(sHowMany);


        if(nHowMany > 0 && nHowMany <= MAX_HOW_MANY_PER_CHOICE)
        {
			printf("%d\t\t%s\t\t%d\n", nChoice + 1, availableItems[nChoice].itemName, nHowMany);
            addOrder(nChoice, nHowMany);
            
            return 0; //0 means success, so allow the caller to continue on 0
        }
        else
        {
            printf("\n%d many items not available for order\n", nHowMany);
        }
    }
    
    
    printf("Invalid Choice, please try again\n");
    displayMenu();
    return takeOrder();
    
}

int main()
{
 
    int nContinue = 0;   
    do
    {
        displayMenu();
        
        int result = takeOrder(); //if takeorder returns 0, there is no error and we proceed
        
        if(result == 0)
        {
            printf("\nDo you want to order more items?  y/n:");
            
            char sContinue[128];
            fgets(sContinue, 128, stdin); //standard input (this is an inbuilt standard file provided by C) 
            
            nContinue = sContinue[0]; 
        }
    }while(nContinue == 'y' || nContinue == 'Y');

    printOrderSummary();
    return 0;
}

