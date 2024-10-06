## Leto Project Contribution Guidelines
This project is in the public domain, meaning that anyone can contribute to its code. However, in order to keep things organized, we must put in place some rules/regulations on _how_ this editing shall be done.

Index:
- [Public Interfaces](#public-interface-design)
- [Interface Implementation](#interface-implementation)
- [Code Styling](#code-styling)
- [Story Contribution](#story-contributions)
- [Interaction Within the Community](#interaction-within-the-community)

### Documentation
All changes, once merged from pull request, will spend a brief time in the `dev` branch before being merged to main. This is an important precaution, as we must make sure that any and all content on the `main` branch is stable on all platforms. One caveat of this is that every interface pushed to `main` _must be documented_. Be it private functions in implementation files, static variables, or files, all must be documented properly before being merged to `main`. Undocumented interfaces _will stall the merge until documented_, so it's better for both maintainers and users if your pull request just documents itself before being merged.

Documentation must be formatted properly. For files:

```c
/**
 * @file [FILENAME]
 * @author [NAME / PSEUDONYM] ([GITHUB LINK])
 * @brief [DESCRIPTION]
 * @implements [PUBLIC INTERFACE (only for implementation files)]
 * @date [DATE]
 *
 * @copyright (c) 2024 - the Leto Team
 * This source code is under the AGPL v3.0. For information on what that
 * entails, please see the attached @file LICENSE.md file.
 */
```

For functions:

```c
/**
 * [FUNCTION NAMES]
 * @author [NAME / PSUEDONYM] ([GITHUB LINK])
 * @brief [DESCRIPTION]
 * 
 * @param [PARAM_NAME] -- [PARAM_USE]
 * @return [RETURN_TYPE] -- [RETURN_VALUE]
 */
```

For variables, types, or macros:

```c
/**
 * @brief [VARIABLE_USE]
 */
```

### Public Interface Design
A new public interface should be created if and only if the functionality cannot be merged into an already existing interface. If you see something that looks like it could be merged, create a pull request and maintainers will take a look at it. These interfaces must be organized and clearly documented. Firstly, an include guard _must_ be on every single public interface you define. This include guard must be in the following format:

```c
#define LETO__[INTERFACE_NAME]_H
#define LETO__[INTERFACE_NAME]_H

#endif // LETO__[INTERFACE_NAME]_H
```

The `endif` must be clearly commented to avoid confusion, regardless of how long the public interface is.

Functions must be clearly named and begin with `Leto`. All functions must be in `PascalCase`. Variables, on the contrary (rare as they should be in public interfaces) should be named via `c_case`. Macros should be in `CONSTANT_CASE` unless there is a specific reason they're not, like the `leto_true_t` macro.

All structures and/or enums must be defined with the following structure:

```c
typedef [enum/struct] [TYPE_NAME_NO_T] {
    // Members
} [TYPE_NAME_WITH_T];
```

For example:

```c
typedef struct example_struct {
    uint8_t some_var;
} example_struct_t;
```

All structures and enums must also be labeled in `c_case`.

Any functions that should not be called directly, like a function whose parameters are provided via preprocessor macro, should be ended with an underscore. These kinds of functions should be avoided, however. Any "private" functions should instead be put in an implementation file.

### Interface Implementation
Each public interface should have exactly one (1) implementation file. If the need for any more arises the interface should be reviewed for splitting. Implementation files should _never_ be `#include`d. If one is, the architecture of the interface should be brought under question and maintainers will decide whether to move forward.

Any and all variables or private functions declared within an implementation file should be declared `static`, although private variables should be avoided. Private functions, however, are encouraged, as it can improve readability and better define what purpose a function carries out. Note that this repository is of the philosophy that if it doesn't _need_ to be public, it _shouldn't_ be public.

Variables should be in `c_case` and private functions should be in `PascalCase` with an underscore added to the end of the function identifier. There is no need to add "Leto" to the beginning of these functions. Example:

```c
#include "ExampleInterface.h"

static int example_var = 12;

static void ExamplePrivateFunction_(void) 
{
    // Do stuff
}

void LetoExamplePublicFunction(void) 
{
    // Do stuff
}

```

Note that all static variables and functions should be placed above any implemented public functions.

### Code Styling
The project comes with a `.clang-format` file. This file defines all the styles that contributors should follow at all times. Any and all changes to this file _must_ be run by a **lead** maintainer. The person who presents the change to the format document must also be the one to convert every file in the repository to the new format.

### Story Contributions
Story and code are very separate within this project. Only specific people may contribute "story content"; this is defined as any content that will change the narrative of Leto. This does _not_ mean mechanics, or improvements to models, or something in that vein. While those changes must be signed off on by lead maintainers (due to the intense changes that would bring to UX), they are not story content, and anyone can propose them.

The procedure for gaining the ability to change story content will be different from person to person, but generally it will involve being extremely active in the community and demonstrating that you understand the current story (and its meanings) well.

### Interaction Within the Community
Leto is a game that many may consider "edgy" or "dark", but we will not tolerate a community of "edgelords". It is a-ok to discuss current news and other things like that on our forums, even if said news is rather morbid, but sharing any content like gore or pornography is _strictly_ forbidden and will be punished with a permanent communication ban on all official sites. Note that lead maintainers reserve the right to remove any content promoting harmful ideologies such as war glorification.

No fighting will be tolerated between maintainers. We are not children. If a conflict arises, it will be solved civily and according to all due process. Differing philosphies will be solved via popular vote by all maintainers.

If criminal allegations are brought against a maintainer, God forbid, those allegations will be dealt with at the discretion of the lead maintainers, often with a permanent ban from all interactions and public posts detailing the punishment. However, we are not in the business of false accusations, and any such made will be punished severly. We are not a forgiving people when it comes to these topics.

None of the lead maintainers / project leads are social people, and as such do not have personal social media accounts, beyond the lead developer's devlog channel on Youtube. If any accounts are made mimicking us, unless we specifically post something on an official site verifying said account, understand that it is fake and does not represent us.
