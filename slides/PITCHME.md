# Bowling Kata - C++

Krystian Piękoś

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 1]

@snap[center text-35 text-white]
**Step 1**
@snapend

@snap[center text-25 text-white fragment]
Start
@snapend

+++

Let's write a first test:

```cpp
TEST(BowlingGameTests, NewGameScore)
{
    BowlingGame game;

    ASSERT_EQ(game.score(), 0);
}
```

+++

Now let's implement the simplest code for this test:

```cpp
class BowlingGame
{
public:
    unsigned int score() const
    {
        return 13;
    }
};
```

+++

Let's compile and run a test:

```bash
[ RUN      ] BowlingGameTests.NewGameScore
..\tests\bowling_game_tests.cpp(8): error: Expected equality of these values:
  game.score()
    Which is: 13
  0
[  FAILED  ] BowlingGameTests.NewGameScore (1 ms)
```

+++

@snap[center text-purple text-15]
**Failing test confirms that the new test is executed in a test suite!!!**
@snapend

+++

After a simple change:

```cpp
class BowlingGame
{
public:
    unsigned int score() const
    {
        return 0;
    }
};
```
@[6]

+++

@snap[center]
we run our test and now it is passed<br/>
@snapend

```bash
[ RUN      ] BowlingGameTests.NewGameScore
[       OK ] BowlingGameTests.NewGameScore (0 ms)
```

@fa[thumbs-o-up green fa-3x]

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 2]

@snap[center text-35 text-white]
**Step 2**
@snapend

@snap[center text-25 text-white fragment]
All rolls in a gutter
@snapend

+++

Second test:

```c++
TEST(BowlingGameTests, AllRollsInAGutter)
{
    BowlingGame game;

    for(int i = 0; i < 20; ++i)
        game.roll(0);
    
    ASSERT_EQ(game.score(), 0);
}
```

+++

Trying to compile we will get an error:

```bash
[build] ..\tests\bowling_game_tests.cpp(16): error C2039: 'roll': is not a member of 'BowlingGame'
```

+++

After adding a new method to ``BowlingGame`` 

```cpp
class Game
{
public:        
    size_t score() const;

    void roll(unsigned int pins)
    {        
    }
};
```

class test passes.

@fa[thumbs-o-up green fa-3x]

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 3]

@snap[center text-35 text-white]
**Step 3**
@snapend

@snap[center text-25 text-white fragment]
No marks
@snapend

+++

Third test:

```c++
TEST(BowlingGameTests, WhenAllRollsNoMarkScoreIsSumOfPins)
{
    BowlingGame game;

    for(int i = 0; i < 20; ++i)
        game.roll(2);
    
    ASSERT_EQ(game.score(), 40);
}
```

fails with a message:

```bash
[ RUN      ] BowlingGameTests.WhenAllRollsNoMarkScoreIsSumOfPins
..\tests\bowling_game_tests.cpp(28): error: Expected equality of these values:
  game.score()
    Which is: 0
  40
[  FAILED  ] BowlingGameTests.WhenAllRollsNoMarkScoreIsSumOfPins (1 ms)
```

+++

Let's write some code to pass a test:

```c++
class BowlingGame
{
    unsigned int score_ = 0;
public:
    unsigned int score() const
    {
        return score_;
    }

    void roll(unsigned int pins)
    {
        score_ += pins;
    }
};
```

Test passes but we are not done yet!

+++

@snap[center text-purple text-15]
**We can refactor the code by removing duplication in tests.**
@snapend

+++

Let's create a fixture class with a helper method:

```c++
class BowlingGameTests : public ::testing::Test
{
protected:
    BowlingGame game;
    
    void roll_many(unsigned int count, unsigned int pins)
    {
        for(auto i = 0u; i < count; ++i)
            game.roll(pins);
    }
};
```

+++

Now we can use this fixture in our tests:

```c++
TEST_F(BowlingGameTests, AllRollsInAGutter)
{
    roll_many(20, 0);
    
    ASSERT_EQ(game.score(), 0);
}

TEST_F(BowlingGameTests, WhenAllRollsNoMarkScoreIsSumOfPins)
{
    roll_many(20, 2);
    
    ASSERT_EQ(game.score(), 40);
}
```

+++

After refactoring all tests pass.

@fa[thumbs-o-up green fa-3x]

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 4]

@snap[center text-35 text-white]
**Step 4**
@snapend

@snap[center text-25 text-white fragment]
Spare
@snapend

+++

Test for a spare:

```c++
TEST_F(BowlingGameTests, Spare)
{
    // spare
    game.roll(1);
    game.roll(9);

    roll_many(18, 1);

    ASSERT_EQ(game.score(), 29); 
}
```

fails:

```bash
[ RUN      ] BowlingGameTests.Spare
..\tests\bowling_game_tests.cpp(43): error: Expected equality of these values:
  game.score()
    Which is: 28
  29
[  FAILED  ] BowlingGameTests.Spare (1 ms)
```

+++

Let's implement a spare feature:

```c++
class BowlingGame {
    std::array<unsigned int, 20> pins_ = {{}};
    size_t roll_no = 0;

public:
    unsigned int score() const {
        unsigned int result{};

        for(size_t i = 0; i < 20; i+=2) {
            if (pins_[i] + pins_[i+1] == 10)
                result += pins_[i + 2];            

            result += pins_[i] + pins_[i+1];
        }
        
        return result;
    }

    void roll(unsigned int pins) {
        pins_[roll_no] = pins;
        ++roll_no;
    }
};
```
@[2-3]
@[7-16]
@[20-21]

+++

Now test is passed. Let's refactor some code:

+++

Simplified implementation of score:

```c++
unsigned int score() const
{
    unsigned int result{};

    for(size_t i = 0; i < 20; i+=2) {          
        result += frame_score(i);

        if (is_spare(i))
            result += spare_bonus(i);
    }
    
    return result;
}
```

+++

Private functions extracted from previous implementation:

```c++
bool is_spare(size_t roll_index) const {
    return pins_[roll_index] + pins_[roll_index + 1] == 10;
}

size_t spare_bonus(size_t roll_index) const {
    return pins_[roll_index + 2];
}

size_t frame_score(size_t roll_index) const {
    return pins_[roll_index] + pins_[roll_index+1];
}

```

+++

We also refactor code of the test:

```c++
TEST_F(BowlingGameTests, Spare)
{
    roll_spare();
    roll_many(18, 1);

    ASSERT_EQ(game.score(), 29); 
}
```

and the fixture:

```c++
class BowlingGameTests : public ::testing::Test
{
protected:
    BowlingGame game;

    //...
   
    void roll_spare()
    {
        game.roll(1);
        game.roll(9);
    }
};
```

+++

Let's run a test suite.

@fa[thumbs-o-up green fa-3x]

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 5]

@snap[center text-35 text-white]
**Step 5**
@snapend

@snap[center text-25 text-white fragment]
Strike
@snapend

+++

Let's write a test for a strike:

```c++
TEST_F(BowlingGameTests, Strike)
{
    roll_many(2, 1);
    roll_strike();
    roll_many(16, 1);

    ASSERT_EQ(game.score(), 30);
}
```

```
void roll_strike()
{
    game.roll(10);
}
```

```bash
[ RUN      ] BowlingGameTests.Strike
..\tests\bowling_game_tests.cpp(61): error: Expected equality of these values:
  game.score()
    Which is: 28
  30
[  FAILED  ] BowlingGameTests.Strike (1 ms)
```

+++

To pass a test we change ``score()`` method:

```c++
unsigned int score() const
{
    size_t result{};
    size_t roll_index = 0;

    for (size_t i = 0; i < 10; ++i) {
        if (is_strike(roll_index)) {
            result += 10 + strike_bonus(roll_index);
            ++roll_index;
        }
        else {
            result += frame_score(roll_index);
            if (is_spare(roll_index))
                result += spare_bonus(roll_index);

            roll_index += 2;
        }
    }

    return result;
}
```
@[6-11,17]

+++

Do we need refactoring?

+++

@snap[center text-purple text-15]
**Let's remove magic constants**
@snapend

+++

Let's introduce a new static members:

```cpp
class BowlingGame {
private:
    constexpr static size_t frames_count = 10;
    constexpr static size_t all_pins_in_frame = 10;

    bool is_strike(size_t roll_index) const
    {
        return pins_[roll_index] == all_pins_in_frame;
    }

    bool is_spare(size_t roll_index) const
    {
        return pins_[roll_index] + pins_[roll_index + 1] 
            == all_pins_in_frame;
    }
};
```
@[3,4]
@[8, 13-14]

+++

Iteration over frames with the new constant:

```cpp
for (size_t i = 0; i < frames_count; ++i)
{
    if (is_strike(roll_index))
    {
        result += 10 + strike_bonus(roll_index);
        ++roll_index;
    }
    else
    {
        result += frame_score(roll_index);
        if (is_spare(roll_index))
            result += spare_bonus(roll_index);

        roll_index += 2;
    }
}
```
@[1]

+++

Let's run a test suite.

@fa[thumbs-o-up green fa-3x]

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 6]

@snap[center text-35 text-white]
**Step 6**
@snapend

@snap[center text-25 text-white fragment]
Spare in the last frame
@snapend

+++

Test comes first:

```
TEST_F(BowlingGameTests, SpareInLastFrame)
{
    roll_many(18, 1);
    roll_spare();
    game.roll(6);

    ASSERT_EQ(game.score(), 34);
}
```

+++

@snap[center]
When running a test suite the program crashes... 
<br/>
<br/>
@fa[exclamation-triangle red fa-3x]
@snapend
+++

We have to increase the size of an array with rolls:

```c++
class BowlingGame
{
    constexpr static size_t frames_count = 10;
    constexpr static size_t all_pins_in_frame = 10;

    std::array<unsigned int, 21> pins_ = {{}};
    size_t roll_no = 0;

    //...
};
```
@[6]

+++

All our tests pass!

@fa[thumbs-o-up green fa-3x]

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 7]

@snap[center text-35 text-white]
**Step 7**
@snapend

@snap[center text-25 text-white fragment]
Strike in the last frame
@snapend

+++

Test for a strike in the last frame:

```c++
TEST_F(BowlingGameTests, StrikeInLastFrame)
{
    using namespace ::testing;

    roll_many(18, 1);
    roll_strike();
    game.roll(6);
    game.roll(7);

    ASSERT_THAT(game.score(), Eq(41));
}
```

+++

Test passes! Let's make some refactoring.

+++

1. Let's refactor tests

```c++
class LastFrameTests : public BowlingGameTests
{
protected:
    void SetUp() override
    {
        roll_many(18, 1);
    }
};
```

+++

```c++
TEST_F(LastFrameTests, SpareInLastFrame)
{
    roll_spare();
    game.roll(6);

    ASSERT_EQ(game.score(), 34);
}

TEST_F(LastFrameTests, StrikeInLastFrame)
{
    using namespace ::testing;

    roll_strike();
    game.roll(6);
    game.roll(7);

    ASSERT_THAT(game.score(), Eq(41));
}
```

+++

2. Let's refactor ``score()`` method in the ``BowlingGame``

```c++
unsigned int score() const
{
    size_t result{};
    size_t roll_index = 0;

    for (size_t i = 0; i < frames_count; ++i)
    {
        auto [score_in_frame, rolls_in_frame] = frame_score(roll_index);

        result += score_in_frame;
        roll_index += rolls_in_frame;
    }

    return result;
}
```

+++

```c++
auto frame_score(size_t roll_index) const
{
    size_t score_in_frame = 0;
    size_t rolls_in_frame = 2;

    if (is_strike(roll_index))
    {
        score_in_frame += all_pins_in_frame + strike_bonus(roll_index);
        rolls_in_frame = 1;
    }
    else
    {
        score_in_frame = pins_[roll_index] + pins_[roll_index + 1];

        if (is_spare(roll_index))
        {
            score_in_frame += spare_bonus(roll_index);
        }
    }

    return std::pair{score_in_frame, rolls_in_frame};
}
```

+++

All our tests pass!

@fa[thumbs-o-up green fa-3x]

---?color=linear-gradient(to left, #0575E6, #021B79)

@title[Step 8]

@snap[center text-35 text-white]
**Step 8**
@snapend

@snap[center text-25 text-white fragment]
Perfect game
@snapend

+++

Let's write a test for a perfect bowling game:

```c++
TEST_F(BowlingGameTests, PerfectGame)
{
    roll_many(12, 10);

    ASSERT_EQ(game.score(), 300);
}
```

+++

All our tests pass!

@fa[thumbs-o-up green fa-3x]
