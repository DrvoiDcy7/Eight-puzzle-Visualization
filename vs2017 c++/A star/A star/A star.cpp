// A star.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include <stdlib.h>
#include <conio.h>

#define size 3
using namespace std;


typedef int status[size][size];
struct SpringLink;


typedef struct Node
{
	status data;
	struct Node *parent;
	struct SpringLink *child;
	struct Node *next;
	int fvalue;
	int gvalue;
	int hvalue;
}NNode, *PNode;



typedef struct SpringLink
{
	struct Node *pointData;
	struct SpringLink *next;
}SPLink, *PSPLink;


PNode open;
PNode closed;



status startt = { 2 , 8 , 3 , 1 , 6 , 4 , 7 , 0 , 5 };
status target = { 1 , 2 , 3 , 8 , 0 , 4 , 7 , 6 , 5 };



void initLink(PNode &Head)
{
	Head = (PNode)malloc(sizeof(NNode));
	Head->next = NULL;
}


bool isEmpty(PNode Head)
{
	if (Head->next == NULL)
		return true;
	else
		return false;
}



void popNode(PNode &Head, PNode &FNode)
{
	if (isEmpty(Head))
	{
		FNode = NULL;
		return;
	}
	FNode = Head->next;
	Head->next = Head->next->next;
	FNode->next = NULL;
}




void addSpringNode(PNode &Head, PNode newData)
{
	PSPLink newNode = (PSPLink)malloc(sizeof(SPLink));
	newNode->pointData = newData;

	newNode->next = Head->child;
	Head->child = newNode;
}



void freeSpringLink(PSPLink &Head)
{
	PSPLink tmm;

	while (Head != NULL)
	{
		tmm = Head;
		Head = Head->next;
		free(tmm);
	}
}



void freeLink(PNode &Head)
{
	PNode tmn;

	tmn = Head;
	Head = Head->next;
	free(tmn);

	while (Head != NULL)
	{
		freeSpringLink(Head->child);
		tmn = Head;
		Head = Head->next;
		free(tmn);
	}
}


void addNode(PNode &Head, PNode &newNode)
{
	newNode->next = Head->next;
	Head->next = newNode;
}



void addAscNode(PNode &Head, PNode &newNode)
{
	PNode P;
	PNode Q;

	P = Head->next;
	Q = Head;
	while (P != NULL && P->fvalue < newNode->fvalue)
	{
		Q = P;
		P = P->next;
	}
	
	newNode->next = Q->next;
	Q->next = newNode;
}



int computeHValue(PNode theNode)
{
	int num = 0;

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (theNode->data[i][j] != target[i][j])
				num++;
		}
	}
	return num;
}



void computeAllValue(PNode &theNode, PNode parentNode)
{
	if (parentNode == NULL)
		theNode->gvalue = 0;
	else
		theNode->gvalue = parentNode->gvalue + 1;

	theNode->hvalue = computeHValue(theNode);
	theNode->fvalue = theNode->gvalue + theNode->hvalue;
}




void initial()
{
	
	initLink(open);
	initLink(closed);

	
	PNode NULLNode = NULL;
	PNode Start = (PNode)malloc(sizeof(NNode));
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Start->data[i][j] = startt[i][j];
		}
	}
	Start->parent = NULL;
	Start->child = NULL;
	Start->next = NULL;
	computeAllValue(Start, NULLNode);

	
	addAscNode(open, Start);
}



void statusAEB(PNode &ANode, PNode BNode)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ANode->data[i][j] = BNode->data[i][j];
		}
	}
}



bool hasSameStatus(PNode ANode, PNode BNode)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (ANode->data[i][j] != BNode->data[i][j])
				return false;
		}
	}
	return true;
}




bool hasAnceSameStatus(PNode OrigiNode, PNode AnceNode)
{
	while (AnceNode != NULL)
	{
		if (hasSameStatus(OrigiNode, AnceNode))
			return true;
		AnceNode = AnceNode->parent;
	}
	return false;
}


void getPosition(PNode theNode, int &row, int &col)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (theNode->data[i][j] == 0)
			{
				row = i;
				col = j;
				return;
			}
		}
	}
}


void changeAB(int &A, int &B)
{
	int C;
	C = B;
	B = A;
	A = C;
}



bool inLink(PNode spciNode, PNode theLink, PNode &theNodeLink, PNode &preNode)
{
	preNode = theLink;
	theLink = theLink->next;

	while (theLink != NULL)
	{
		if (hasSameStatus(spciNode, theLink))
		{
			theNodeLink = theLink;
			return true;
		}
		preNode = theLink;
		theLink = theLink->next;
	}
	return false;
}



void SpringLink(PNode theNode, PNode &spring)
{
	int row;
	int col;

	getPosition(theNode, row, col);

	
	if (col != 2)
	{
		PNode rlNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(rlNewNode, theNode);
		changeAB(rlNewNode->data[row][col], rlNewNode->data[row][col + 1]);
		if (hasAnceSameStatus(rlNewNode, theNode->parent))
		{
			free(rlNewNode);
		}
		else
		{
			rlNewNode->parent = theNode;
			rlNewNode->child = NULL;
			rlNewNode->next = NULL;
			computeAllValue(rlNewNode, theNode);
			addNode(spring, rlNewNode);
		}
	}
	if (col != 0)
	{
		PNode lrNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(lrNewNode, theNode);
		changeAB(lrNewNode->data[row][col], lrNewNode->data[row][col - 1]);
		if (hasAnceSameStatus(lrNewNode, theNode->parent))
		{
			free(lrNewNode);
		}
		else
		{
			lrNewNode->parent = theNode;
			lrNewNode->child = NULL;
			lrNewNode->next = NULL;
			computeAllValue(lrNewNode, theNode);
			addNode(spring, lrNewNode);
		}
	}
	
	if (row != 0)
	{
		PNode udNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(udNewNode, theNode);
		changeAB(udNewNode->data[row][col], udNewNode->data[row - 1][col]);
		if (hasAnceSameStatus(udNewNode, theNode->parent))
		{
			free(udNewNode);
		}
		else
		{
			udNewNode->parent = theNode;
			udNewNode->child = NULL;
			udNewNode->next = NULL;
			computeAllValue(udNewNode, theNode);
			
			addNode(spring, udNewNode);
		}
	}
	
	if (row != 2)
	{
		PNode duNewNode = (PNode)malloc(sizeof(NNode));
		statusAEB(duNewNode, theNode);
		changeAB(duNewNode->data[row][col], duNewNode->data[row + 1][col]);
		if (hasAnceSameStatus(duNewNode, theNode->parent))
		{
			free(duNewNode);
		}
		else
		{
			duNewNode->parent = theNode;
			duNewNode->child = NULL;
			duNewNode->next = NULL;
			computeAllValue(duNewNode, theNode);
			addNode(spring, duNewNode);
		}
	}
}



void outputStatus(PNode stat)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cout << stat->data[i][j] << " ";
		}
		cout << endl;
	}
}



void outputBestRoad(PNode goal)
{
	int deepnum = goal->gvalue;

	if (goal->parent != NULL)
	{
		outputBestRoad(goal->parent);
	}
	cout << "The" << deepnum-- << "st layer's situation:" << endl;
	outputStatus(goal);
}


void AStar()
{
	PNode tmpNode;//Pointer of the node taken from table open into table closed 
	PNode spring;
	PNode tmpLNode;
	PNode tmpChartNode;
	PNode thePreNode;
	bool getGoal = false;
	long numcount = 1;

	initial();
	initLink(spring);
	tmpChartNode = NULL;

	cout << "Node taken out from table open" << endl;
	while (!isEmpty(open))
	{
		
		popNode(open, tmpNode);
		addNode(closed, tmpNode);


		cout << "The" << numcount++ << "st situation:" << endl;
		outputStatus(tmpNode);
		cout << "value of f:" << tmpNode->fvalue << endl;
		cout << "value of g:" << tmpNode->gvalue << endl;
		cout << "value of h:" << tmpNode->hvalue << endl;


	
		if (computeHValue(tmpNode) == 0)
		{
			getGoal = true;
			break;
		}

		
		SpringLink(tmpNode, spring);

		while (!isEmpty(spring))
		{
			popNode(spring, tmpLNode);
			
			if (inLink(tmpLNode, open, tmpChartNode, thePreNode))
			{
				addSpringNode(tmpNode, tmpChartNode);
				if (tmpLNode->gvalue < tmpChartNode->gvalue)
				{
					tmpChartNode->parent = tmpLNode->parent;
					tmpChartNode->gvalue = tmpLNode->gvalue;
					tmpChartNode->fvalue = tmpLNode->fvalue;
				}
				free(tmpLNode);
			}
			
			else if (inLink(tmpLNode, closed, tmpChartNode, thePreNode))
			{
				addSpringNode(tmpNode, tmpChartNode);
				if (tmpLNode->gvalue < tmpChartNode->gvalue)
				{
					PNode commu;
					tmpChartNode->parent = tmpLNode->parent;
					tmpChartNode->gvalue = tmpLNode->gvalue;
					tmpChartNode->fvalue = tmpLNode->fvalue;
					freeSpringLink(tmpChartNode->child);
					tmpChartNode->child = NULL;
					popNode(thePreNode, commu);
					addAscNode(open, commu);
				}
				free(tmpLNode);
			}
			
			else
			{
				addSpringNode(tmpNode, tmpLNode);
				addAscNode(open, tmpLNode);
			}
		}
	}

	
	if (getGoal)
	{
		cout << endl;
		cout << "Length of path：" << tmpNode->gvalue << endl;
		outputBestRoad(tmpNode);
	}


	freeLink(open);
	freeLink(closed);
	getch();
}


int main()
{
	AStar();
	return 0;
}

