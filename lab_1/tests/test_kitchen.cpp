#include <gtest/gtest.h>
#include "Kitchen.hpp"

TEST(CookTest, CookStatus) {
    Cook cook(1);

    EXPECT_EQ(cook.getCookId(), 1);
    EXPECT_FALSE(cook.isBusy());
    cook.startCooking();
    EXPECT_TRUE(cook.isBusy());
    cook.finishCooking();
    EXPECT_FALSE(cook.isBusy());
}

TEST(CookTest, WrongIdCook) {
    EXPECT_THROW(Cook cook(-1), std::invalid_argument);
}

TEST(CookTest, CookBusy) {
    Cook cook(1);
    EXPECT_THROW(cook.finishCooking(), std::logic_error);
    cook.startCooking();
    EXPECT_THROW(cook.startCooking(), std::logic_error);
}



TEST(WorkplaceTest, PlaceStatus) {
    Workplace place(1);

    EXPECT_EQ(place.getWorkplaceId(), 1);
    EXPECT_FALSE(place.isBusy());
    place.occupy();
    EXPECT_TRUE(place.isBusy());
    place.release();
    EXPECT_FALSE(place.isBusy());
}

TEST(WorkplaceTest, WrongIdPlace) {
    EXPECT_THROW(Workplace place(-1), std::invalid_argument);
}

TEST(WorkplaceTest, PlaceBusy) {
    Workplace place(1);
    EXPECT_THROW(place.release(), std::logic_error);
    place.occupy();
    EXPECT_THROW(place.occupy(), std::logic_error);
}



TEST(StorageTest, EmptyStorage) {
    Storage storage({});
    EXPECT_TRUE(storage.getStore().empty());
}

TEST(StorageTest, StorageDefine) {
    Storage storage({
        {Ingredient::Bread, 100}
    });

    EXPECT_EQ(storage.getStore().size(), 1);
}

TEST(StorageTest, StorageNegative) {
    EXPECT_THROW(Storage storage({{Ingredient::Bread, -100}}), std::invalid_argument);
}

TEST(StorageTest, Storage_Has_Ingredients) {
    Storage storage({
        {Ingredient::Bread, 0}
    });

    Recipe coctail(
        "Coctail",
        {
            {Ingredient::Milk, 100},
        },
        5
    );
    EXPECT_EQ(storage.getStore().size(), 1);
    EXPECT_FALSE(storage.hasIngredients(coctail));
    storage.restock(Ingredient::Milk, 1);
    EXPECT_FALSE(storage.hasIngredients(coctail));
    storage.restock(Ingredient::Milk, 300);
    EXPECT_EQ(storage.getStore().size(), 2);
    EXPECT_TRUE(storage.hasIngredients(coctail));
    EXPECT_THROW(storage.restock(Ingredient::Bread, -100), std::invalid_argument);
}

TEST(StorageTest, Storage_Consume_Ingredients) {
    Storage storage({
        {Ingredient::Milk, 1}
    });

    Recipe coctail(
        "Coctail",
        {
            {Ingredient::Milk, 100},
        },
        5
    );
    EXPECT_THROW(storage.consumeIngredients(coctail), std::logic_error);
    EXPECT_EQ(storage.getStore().at(Ingredient::Milk), 1);
    storage.restock(Ingredient::Milk, 300);
    storage.consumeIngredients(coctail);
    EXPECT_EQ(storage.getStore().at(Ingredient::Milk), 201);
}



TEST(KitchenTest, FreeKitchenResources) {
    Storage storage({});
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    EXPECT_NE(kitchen.findFreeCook(), nullptr);
    EXPECT_NE(kitchen.findFreePlace(), nullptr);
}

TEST(KitchenTest, BusyKitchenResources) {
    Storage storage({});
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    kitchen.findFreeCook()->startCooking();
    EXPECT_EQ(kitchen.findFreeCook(), nullptr);
    kitchen.findFreePlace()->occupy();
    EXPECT_EQ(kitchen.findFreePlace(), nullptr);
}

TEST(KitchenTest, EmptyKitchen) {
    Storage storage({});
    Kitchen kitchen({}, {}, {storage});

    EXPECT_EQ(kitchen.findFreeCook(), nullptr);
    EXPECT_EQ(kitchen.findFreePlace(), nullptr);
}

TEST(KitchenTest, StartCooking) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);

    EXPECT_TRUE(kitchen.TryStartCooking(coctail, 0));
    EXPECT_EQ(coctail.getStatus(), DishStatus::Cooking);
    EXPECT_EQ(kitchen.findFreeCook(), nullptr);
    EXPECT_EQ(kitchen.findFreePlace(), nullptr);
}

TEST(KitchenTest, NotEnoughResources) {
    Storage storage({
        {Ingredient::Milk, 1}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);

    EXPECT_FALSE(kitchen.TryStartCooking(coctail, 0));
    EXPECT_EQ(coctail.getStatus(), DishStatus::Waiting);
}

TEST(KitchenTest, NotEnoughCook) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);
    kitchen.findFreeCook()->startCooking();

    EXPECT_FALSE(kitchen.TryStartCooking(coctail, 0));
    EXPECT_EQ(coctail.getStatus(), DishStatus::Waiting);
    EXPECT_EQ(kitchen.findFreeCook(), nullptr);
}

TEST(KitchenTest, NotEnoughPlaces) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);
    kitchen.findFreePlace()->occupy();

    EXPECT_FALSE(kitchen.TryStartCooking(coctail, 0));
    EXPECT_EQ(coctail.getStatus(), DishStatus::Waiting);
    EXPECT_EQ(kitchen.findFreePlace(), nullptr);
}

TEST(KitchenTest, TryStartDishTwice) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1), Cook(2)}, {Workplace(1), Workplace(2)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);

    kitchen.TryStartCooking(coctail, 0);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Cooking);
    EXPECT_THROW(kitchen.TryStartCooking(coctail, 1), std::logic_error);
}

TEST(KitchenTest, TryStartReadyDish) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);

    coctail.setStatus(DishStatus::Cooking);
    coctail.setStatus(DishStatus::Ready);

    EXPECT_THROW(kitchen.TryStartCooking(coctail, 0), std::logic_error);
}

TEST(KitchenTest, FinishEarlierCooking) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);

    EXPECT_TRUE(kitchen.TryStartCooking(coctail, 0));
    kitchen.finishCooking(5);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Cooking);
    EXPECT_EQ(kitchen.findFreeCook(), nullptr);
    EXPECT_EQ(kitchen.findFreePlace(), nullptr);
}

TEST(KitchenTest, CorrectFinish) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);


    kitchen.TryStartCooking(coctail, 0);
    kitchen.finishCooking(10);
    EXPECT_NE(kitchen.findFreeCook(), nullptr);
    EXPECT_NE(kitchen.findFreePlace(), nullptr);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Ready);
}

TEST(KitchenTest, CorrectFinish2) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);


    kitchen.TryStartCooking(coctail, 0);
    kitchen.finishCooking(10);
    kitchen.finishCooking(10);
    EXPECT_NE(kitchen.findFreeCook(), nullptr);
    EXPECT_NE(kitchen.findFreePlace(), nullptr);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Ready);
}

TEST(KitchenTest,FinishDishWhileAnotherCooking) {
    Storage storage({
        {Ingredient::Milk, 100},
        {Ingredient::Fish, 100}
    });
    Kitchen kitchen({Cook(1), Cook(2)}, {Workplace(1), Workplace(2)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Recipe recipe2("Strips", {{Ingredient::Fish, 100}}, 7);
    Dish coctail(recipe);
    Dish strips(recipe2);

    kitchen.TryStartCooking(coctail, 0);
    kitchen.TryStartCooking(strips, 0);
    kitchen.finishCooking(7);
    EXPECT_EQ(strips.getStatus(), DishStatus::Ready);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Cooking);
    EXPECT_NE(kitchen.findFreePlace(), nullptr);
    EXPECT_NE(kitchen.findFreeCook(), nullptr);
    kitchen.finishCooking(10);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Ready);
}

TEST(KitchenTest, TwoDishesSameTime) {
    Storage storage({
        {Ingredient::Milk, 100},
        {Ingredient::Fish, 100}
    });
    Kitchen kitchen({Cook(1), Cook(2)}, {Workplace(1), Workplace(2)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Recipe recipe2("Strips", {{Ingredient::Fish, 100}}, 10);
    Dish coctail(recipe);
    Dish strips(recipe2);

    kitchen.TryStartCooking(coctail, 0);
    kitchen.TryStartCooking(strips, 0);
    EXPECT_EQ(kitchen.findFreePlace(), nullptr);
    EXPECT_EQ(kitchen.findFreeCook(), nullptr);
    kitchen.finishCooking(10);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Ready);
    EXPECT_EQ(strips.getStatus(), DishStatus::Ready);
    EXPECT_NE(kitchen.findFreePlace(), nullptr);
    EXPECT_NE(kitchen.findFreeCook(), nullptr);
}

TEST(KitchenTest, CorrectSkippedFinish) {
    Storage storage({
        {Ingredient::Milk, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);


    kitchen.TryStartCooking(coctail, 0);
    kitchen.finishCooking(50);
    EXPECT_NE(kitchen.findFreeCook(), nullptr);
    EXPECT_NE(kitchen.findFreePlace(), nullptr);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Ready);
}

TEST(KitchenTest, SecondDishOnlyAfterFirst) {
    Storage storage({
        {Ingredient::Milk, 100},
        {Ingredient::Fish, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);
    Recipe recipe2("Strips", {{Ingredient::Fish, 100}}, 7);
    Dish strips(recipe2);


    kitchen.TryStartCooking(coctail, 0);
    EXPECT_FALSE(kitchen.TryStartCooking(strips, 5));
    kitchen.finishCooking(10);
    EXPECT_EQ(coctail.getStatus(), DishStatus::Ready);
    EXPECT_TRUE(kitchen.TryStartCooking(strips, 10));
    EXPECT_EQ(kitchen.findFreeCook(), nullptr);
    EXPECT_EQ(kitchen.findFreePlace(), nullptr);
}

TEST(KitchenTest, NoActions) {
    Storage storage({
        {Ingredient::Milk, 100},
        {Ingredient::Fish, 100}
    });
    Kitchen kitchen({Cook(1)}, {Workplace(1)}, {storage});

    Recipe recipe("Pinakolada", {{Ingredient::Milk, 100}}, 10);
    Dish coctail(recipe);
    Recipe recipe2("Strips", {{Ingredient::Fish, 100}}, 7);
    Dish strips(recipe2);


    EXPECT_NO_THROW(kitchen.finishCooking(0));
    EXPECT_NO_THROW(kitchen.finishCooking(10));
    EXPECT_NE(kitchen.findFreeCook(), nullptr);
    EXPECT_NE(kitchen.findFreePlace(), nullptr);
}