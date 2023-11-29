#include <bits/stdc++.h>
using namespace std;

class FamilyNode
{
public:
    vector<FamilyNode *> children;
    string name;
    int age;
    float income;
    string occupation;
};

FamilyNode *buildTree()
{
    string name, occupation;
    int age, numOfChildren;
    float income;
    cout << "Enter name, age, income, and occupation of member" << endl;
    cin >> name >> age >> income >> occupation;

    FamilyNode *node = new FamilyNode();
    node->name = name;
    node->age = age;
    node->income = income;
    node->occupation = occupation;

    cout << "Enter number of children" << endl;
    cin >> numOfChildren;

    for (int i = 0; i < numOfChildren; i++)
    {
        node->children.push_back(buildTree());
    }

    return node;
}

bool checkIfNodeExists(FamilyNode *node, string name, int age, float income, string occupation)
{
    if (node == nullptr)
        return false;

    if (node->name == name && node->age == age && node->income == income && node->occupation == occupation)
        return true;

    for (auto child : node->children)
    {
        if (checkIfNodeExists(child, name, age, income, occupation))
            return true;
    }

    return false;
}

void removeNode(FamilyNode *node, FamilyNode *parent, string name, int age, float income, string occupation)
{
    if (node == nullptr)
        return;

    if (node->name == name && node->age == age && node->income == income && node->occupation == occupation)
    {
        if (parent != nullptr)
        {
            // Remove current node from parent's children
            auto it = find(parent->children.begin(), parent->children.end(), node);
            if (it != parent->children.end())
            {
                parent->children.erase(it);
            }

            // Find the child with the highest income
            FamilyNode *newParent = nullptr;
            float maxChildIncome = 0;

            for (auto child : node->children)
            {
                if (child->income > maxChildIncome)
                {
                    maxChildIncome = child->income;
                    newParent = child;
                }
            }

            // If a child with higher income is found, make it the new parent
            if (newParent != nullptr)
            {
                // Remove new parent from its current position
                auto newParentIt = find(node->children.begin(), node->children.end(), newParent);
                if (newParentIt != node->children.end())
                {
                    node->children.erase(newParentIt);
                }

                // Add new parent to parent's children
                parent->children.push_back(newParent);

                // Append the remaining children of the removed node to the new parent's children
                newParent->children.insert(newParent->children.end(), node->children.begin(), node->children.end());

                delete node;
                return; // Return after deleting the node to avoid further processing
            }
            else
            {
                // If no child with higher income is found, append the children of the removed node to the parent's children
                parent->children.insert(parent->children.end(), node->children.begin(), node->children.end());
            }

            delete node;
            return; // Return after deleting the node to avoid further processing
        }
    }

    for (auto child : node->children)
    {
        removeNode(child, node, name, age, income, occupation);
    }
}

float getTotalIncome(FamilyNode *node)
{
    float totalIncome = node->income;
    for (auto child : node->children)
    {
        totalIncome += getTotalIncome(child);
    }
    return totalIncome;
}

int getTotalAge(FamilyNode *node)
{
    int totalAge = node->age;
    for (auto child : node->children)
    {
        totalAge += getTotalAge(child);
    }
    return totalAge;
}

FamilyNode *getHighestIncomeAgeRatioFamily(vector<FamilyNode *> &familyTrees)
{
    FamilyNode *result = nullptr;
    float maxRatio = 0;

    for (auto node : familyTrees)
    {
        float currFamilyTotalIncome = getTotalIncome(node);
        int currFamilyTotalAge = getTotalAge(node);

        float currFamilyRatio = (currFamilyTotalIncome / currFamilyTotalAge);

        if (currFamilyRatio > maxRatio)
        {
            maxRatio = currFamilyRatio;
            result = node;
        }
    }

    return result;
}

void displayTree(FamilyNode *node, int level = 0)
{
    if (node == nullptr)
        return;

    for (int i = 0; i < level; i++)
    {
        cout << "  "; // Two spaces per level for indentation
    }

    cout << node->name << " (Income: " << node->income << ", Age: " << node->age << ", Occupation: " << node->occupation << ")" << endl;

    for (auto child : node->children)
    {
        displayTree(child, level + 1);
    }
}

void printFamiliesInDecrOrder(vector<FamilyNode *> &familyTrees)
{
    sort(familyTrees.begin(), familyTrees.end(), [](FamilyNode *a, FamilyNode *b) {
        return getTotalIncome(a) > getTotalIncome(b);
    });

    for (auto node : familyTrees)
    {
        cout << node->name << " ";
    }
    cout << endl;
}

int main()
{
    int numOfFamilies;
    cout << "Enter the number of families\n";
    cin >> numOfFamilies;

    vector<FamilyNode *> familyTrees;

    for (int i = 0; i < numOfFamilies; i++)
    {
        familyTrees.push_back(buildTree());
    }

    while (1)
    {
        int choice;
        cout << "1. Remove node" << endl;
        cout << "2. Find Highest Income-To-Age-Ratio Family" << endl;
        cout << "3. Print Family(Ancestor) Names in Decreasing Income Order" << endl;
        cout << "4. Display Family Trees" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter your choice" << endl;

        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int num;
            cout << "Enter Family index from which node to be removed" << endl;
            cin >> num;
            cout << "Enter name, age, income, and occupation of member to be removed" << endl;
            string name, occupation;
            int age;
            float income;
            cin >> name >> age >> income >> occupation;

            if (!checkIfNodeExists(familyTrees[num], name, age, income, occupation))
                cout << "No such family member exists" << endl;
            else
                removeNode(familyTrees[num], nullptr, name, age, income, occupation);
            break;
        }
        case 2:
        {
            FamilyNode *node = getHighestIncomeAgeRatioFamily(familyTrees);
            if (node != nullptr)
                cout << "Name of ancestor of the family with the highest total income is " << node->name << endl;
            else
                cout << "No families found." << endl;
            break;
        }
        case 3:
        {
            printFamiliesInDecrOrder(familyTrees);
            break;
        }
        case 4:
        {
            cout << "Displaying Family Trees:" << endl;
            for (int i = 0; i < numOfFamilies; i++)
            {
                cout << "Family " << i + 1 << ":" << endl;
                displayTree(familyTrees[i]);
                cout << endl;
            }
            break;
        }
        case 5:
        {
            // Clean up allocated memory before exiting
            for (auto node : familyTrees)
            {
                delete node;
            }
            return 0;
        }
        default:
        {
            cout << "Incorrect choice!!!" << endl;
            break;
        }
        }
    }

    return 0;
}



/*
2
a 45 60 o1
2
b 18 75 o2
0
c 19 45 o4
2
d 8 14 o4
0
e 9 16 o5
0
f 1 1000 o9
0
1
0
c 19 45 o4
4
2
3
5*/
