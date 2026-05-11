#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include "Recipe.h"
#include "Chest.h"
#include "CraftEngine.h"
#include "ToolUpgrade.h"
#include "SaveSystem.h"
#include "Data.h"

int getIntInput() {

    int x;

    while (true) {

        std::cin >> x;

        if (!std::cin.fail()) {

            return x;
        }

        std::cout << "Invalid input. Numbers only.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Try again: ";
    }
}

double getDoubleInput() {

    double x;

    while (true) {

        std::cin >> x;

        if (!std::cin.fail()) {

            return x;
        }

        std::cout << "Invalid number.\n";

        std::cin.clear();

        std::cin.ignore(
            std::numeric_limits<std::streamsize>::max(),
            '\n'
        );

        std::cout << "Try again: ";
    }
}


void showItems(std::vector<Recipe>& list) {

    for (int i = 0; i < list.size(); i++) {

        std::cout << "[" << i + 1 << "] "
            << list[i].itemName
            << "\n";
    }
}


void showRecipeRequirements(Chest& chest, Recipe& r) {

    std::cout << "ITEM: " << r.itemName << "\n";
    std::cout << "REQUIRED TOOL: " << r.station << "\n";
    std::cout << "RECIPE TIER: " << r.tier << "\n";

    bool canCraft = true;

    for (auto& ing : r.ingredients) {

        bool found = false;

        for (auto& playerIng : chest.ingredients) {

            if (playerIng.name == ing.name &&
                playerIng.quantity >= ing.quantity) {

                found = true;
                break;
            }
        }

        if (found) {

            std::cout << "[OK] ";
        }
        else {

            std::cout << "[MISSING] ";
            canCraft = false;
        }

        std::cout << ing.name
            << " x"
            << ing.quantity
            << "\n";
    }


    if (!canCraft) {

        std::cout << "You do not have enough materials.\n";
        return;
    }

    int opt;

    std::cout << "\n[1] Craft\n";
    std::cout << "[2] Back\n";
    std::cout << "Choice: ";
    opt = getIntInput();

    if (opt == 1) {

        CraftEngine::craft(chest, r);
    }
}

int main() {

    Chest chest;

    SaveSystem::load(chest, "save.txt");

    if (chest.tools.empty()) {

        chest.tools.push_back(Tool("Forge Hammer", 1));
        chest.tools.push_back(Tool("Mystical Arcane Table", 1));
        chest.tools.push_back(Tool("Guardian Sigil", 1));
    }

    int choice;

    while (true) {

        std::cout << "          MAIN MENU" << '\n';
        std::cout << "[1] Chest" << '\n';
        std::cout << "[2] Craft Engine" << '\n';
        std::cout << "[3] Save & Exit" << '\n';
        std::cout << "Choice: ";

        choice = getIntInput();

        if (choice == 1) {

            int c;

            while (true) {

                std::cout << "          CHEST" << '\n';
                std::cout << "[1] View Inventory" << '\n';
                std::cout << "[2] Add Raw Ingredient" << '\n';
                std::cout << "[3] Back" << '\n';
                std::cout << "Choice: ";

                c = getIntInput();

                if (c == 1) {

                    std::cout << "        INGREDIENTS " << '\n';

                    if (chest.ingredients.empty()) {

                        std::cout << "No ingredients." << '\n';
                    }

                    for (auto& i : chest.ingredients) {

                        std::cout << i.name << " - " << i.quantity << "\n";
                    }

                    std::cout << "        TOOLS " << '\n';

                    if (chest.tools.empty()) {

                        std::cout << "No tools." << '\n';
                    }

                    for (auto& t : chest.tools) {
                        std::cout << t.name << " Tier " << t.tier << '\n';
                    }

                    std::cout << "        ITEMS " << '\n';

                    if (chest.items.empty()) {

                        std::cout << "No crafted items." << '\n';
                    }

                    for (auto& it : chest.items) {

                        std::cout << it.name << "\n";
                    }
                }
                else if (c == 2) {

                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::string name;
                    std::getline(std::cin, name);
                    double qty;

                    std::cout << "Allowed Raw Ingredients:" << '\n';

                    std::cout << "- Iron Ore" << '\n';
                    std::cout << "- Wood" << '\n';
                    std::cout << "- Leather" << '\n';
                    std::cout << "- Cloth" << '\n';
                    std::cout << "- Mana Crystal" << '\n';
                    std::cout << "- Magic Dust" << '\n';
                    std::cout << "- Crystal Shard" << '\n';
                    std::cout << "- Beast Fang" << '\n';
                    std::cout << "- Energy Core" << '\n';
                    std::cout << "- Steel Fragment" << '\n';
                    std::cout << "- Dark Essence" << '\n';

                    std::cout << "Enter raw ingredient name: " << '\n';
                    std::getline(std::cin, name);

                    if (!isValidRawIngredient(name)) {

                        std::cout << "Invalid raw ingredient!" << '\n';
                        continue;
                    }

                    std::cout << "Quantity: ";
                    std::cin >> qty;

                    if (qty <= 0) {

                        std::cout << "Invalid quantity!" << '\n';
                        continue;
                    }

                    chest.addIngredient(
                        Ingredient(name, qty, "pcs")
                    );

                    std::cout << "Ingredient added successfully!" << '\n';
                }

                else if (c == 3) {

                    break;
                }
            }
        }

        else if (choice == 2) {

            int cat;

            while (true) {

                std::cout << "          CRAFT ENGINE" << '\n';
                std::cout << "[1] Basic Items" << '\n';
                std::cout << "[2] Advanced Items" << '\n';
                std::cout << "[3] Superior Items" << '\n';
                std::cout << "[4] Special Items" << '\n';
                std::cout << "[5] Tool Upgrade" << '\n';
                std::cout << "[6] Back" << '\n';
                std::cout << "Choice: ";

                cat = getIntInput();
                if (cat == 1) {

                    std::vector<Recipe> list =
                        getBasicRecipes();

                    std::cout << "           BASIC ITEMS " << '\n';

                    showItems(list);
                    int pick;

                    std::cout << "Select item: ";
                    std::cin >> pick;

                    if (pick < 1 || pick > list.size()) {

                        std::cout << "Invalid selection!" << '\n';
                        continue;
                    }

                    Recipe r = list[pick - 1];

                    showRecipeRequirements(chest, r);
                }
                else if (cat == 2) {

                    std::vector<Recipe> list =
                        getAdvancedRecipes();

                    std::cout << "         ADVANCED ITEMS " << '\n';

                    showItems(list);
                    int pick;

                    std::cout << "Select item: ";
                    std::cin >> pick;

                    if (pick < 1 || pick > list.size()) {

                        std::cout << "Invalid selection!\n";
                        continue;
                    }

                    Recipe r = list[pick - 1];

                    showRecipeRequirements(chest, r);
                }
                else if (cat == 3) {

                    std::vector<Recipe> list =
                        getSuperiorRecipes();

                    std::cout << "\n         SUPERIOR ITEMS \n";

                    showItems(list);

                    int pick;

                    std::cout << "Select item: ";
                    std::cin >> pick;

                    if (pick < 1 || pick > list.size()) {

                        std::cout << "Invalid selection!\n";
                        continue;
                    }

                    Recipe r = list[pick - 1];

                    showRecipeRequirements(chest, r);
                }
                else if (cat == 4) {

                    std::vector<Recipe> list =
                        getSpecialRecipes();

                    std::cout << "          SPECIAL ITEMS " << '\n';

                    showItems(list);

                    int pick;

                    std::cout << "Select item: ";
                    pick = getIntInput();

                    if (pick < 1 || pick > list.size()) {

                        std::cout << "Invalid selection!" << '\n';
                        continue;
                    }

                    Recipe r = list[pick - 1];
                    showRecipeRequirements(chest, r);
                }
                else if (cat == 5) {

                    std::cout << "              TOOLS " << '\n';

                    for (auto& t : chest.tools) {
                        std::cout << t.name << " Tier " << t.tier << '\n';
                    }

                    std::cin.ignore(std::numeric_limits<std::streamsize>::max());

                    std::string name;
                    std::cout << "Enter tool name: " << '\n';
                    std::getline(std::cin, name);
                    ToolUpgrade::upgradeTool(chest, name);
                }
                else if (cat == 6) {
                    break;
                }
            }
        }
        else if (choice == 3) {
            SaveSystem::save(chest, "save.txt");
            std::cout << "Game Saved Successfully!" << '\n';
            break;
        }
    }
    return 0;
}