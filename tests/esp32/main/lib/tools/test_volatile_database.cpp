#include <doctest.h>

#include <volatile_database.hpp>

TEST_CASE("Database length") {
  VolatileDatabase<size_t, 3> database;
  CHECK(database.length() == 3);
}

TEST_CASE("Fetching latest values.") {
  SUBCASE("Empty database") {
    VolatileDatabase<size_t, 5> database;
    CHECK(database.latest() == 0);
    SUBCASE("Add first value") {
      database.add(1);
      CHECK(database.latest() == 1);
      SUBCASE("Add another value") {
        database.add(2);
        CHECK(database.latest() == 2);
      }
    }
  }
}

TEST_CASE("Maximum value") {
  SUBCASE("Empty database") {
    VolatileDatabase<size_t, 5> database;
    CHECK(database.maximum() == 0);
  }

  SUBCASE("Maximum is last element") {
    VolatileDatabase<size_t, 5> database;
    database.add(1);
    database.add(2);
    database.add(8);
    CHECK(database.maximum() == 8);
  }

  SUBCASE("Maximum is first element") {
    VolatileDatabase<size_t, 5> database;
    database.add(8);
    database.add(1);
    database.add(1);
    CHECK(database.maximum() == 8);
  }

  SUBCASE("Maximum is middle element") {
    VolatileDatabase<size_t, 5> database;
    database.add(8);
    database.add(9);
    database.add(1);
    CHECK(database.maximum() == 9);
  }

  SUBCASE("Maximum is duplicate element") {
    VolatileDatabase<size_t, 5> database;
    database.add(9);
    database.add(9);
    database.add(9);
    CHECK(database.maximum() == 9);
  }
}

TEST_CASE("Minimum value") {
  SUBCASE("Empty database") {
    VolatileDatabase<size_t, 5> database;
    CHECK(database.minimum() == 0);
  }

  SUBCASE("Minimum is last element") {
    VolatileDatabase<size_t, 5> database;
    database.add(2);
    database.add(2);
    database.add(1);
    CHECK(database.minimum() == 1);
  }

  SUBCASE("Minimum is first element") {
    VolatileDatabase<size_t, 5> database;
    database.add(1);
    database.add(2);
    database.add(3);
    CHECK(database.minimum() == 1);
  }

  SUBCASE("Minimum is middle element") {
    VolatileDatabase<size_t, 5> database;
    database.add(81);
    database.add(1);
    database.add(3);
    CHECK(database.minimum() == 1);
  }

  SUBCASE("Minimum is duplicate element") {
    VolatileDatabase<size_t, 5> database;
    database.add(9);
    database.add(9);
    database.add(9);
    CHECK(database.minimum() == 9);
  }
}

TEST_CASE("Retrieving values with at") {
  SUBCASE("Empty database") {
    VolatileDatabase<size_t, 2> database;
    CHECK(database.at(0) == 0);
    CHECK(database.at(1) == 0);

    SUBCASE("Add a value") {
      database.add(5);
      CHECK(database.at(0) == 5);
      CHECK(database.at(1) == 0);

      SUBCASE("Add another") {
        database.add(3);
        CHECK(database.at(0) == 3);
        CHECK(database.at(1) == 5);

        SUBCASE("Adding more than capacity acts as queue") {
          database.add(4);
          CHECK(database.at(0) == 4);
          CHECK(database.at(1) == 3);
        }
      }
    }
  }
}

TEST_CASE("Retrieving scaled values.") {
  SUBCASE("Empty database") {
    VolatileDatabase<size_t, 3> database;
    CHECK(database.scaledAt(0, 0, 100) == 0);
    SUBCASE("Scales to minimum") { CHECK(database.scaledAt(1, 1, 100) == 1); }

    SUBCASE("Min and maximum") {
      database.add(8);
      database.add(5);
      database.add(2);
      CHECK(database.scaledAt(0, 20, 120) == 20);
      CHECK(database.scaledAt(1, 20, 120) == 70);
      CHECK(database.scaledAt(2, 20, 120) == 120);
    }
  }

  SUBCASE("Index out of range is equal to minimum") {
    VolatileDatabase<size_t, 2> database;
    database.add(1);
    database.add(2);
    CHECK(database.scaledAt(4, 20, 100) == 20);
  }
}