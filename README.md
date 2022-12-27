# Project 1a : Boggle Solver (in C)

## How to compile

To compile `spellcheck`

```
make spellcheck
```

To compile `onePlayerBoggle`

```
make onePlayerBoggle
```

## Testing using ./test.sh

Note the test file may take some time to run because of `valgrind`. If you are completing this project on replit, this may be quite, quite slow. I recommend you use smaller dictionaries and test different aspects of your program and avoid simply testing with `./teset.sh`

## Help Received

Please document any help you received in completing this lab. Note that the what you submit should be your own work. Refer to the syllabus for more details.

I received help completing this lab in the Monday section of lab with Grady and Marlee, and also received some help during office hours with Dania.
The work that I am submitting is my own, but I received some help talking through the code conceptually, and debugging for some errors.

## Describe your work

Please provide a short description of your programming progress

For the SpellCheck part of the lab, I first tried to draw diagrams of what was happening conceptually. For example, in lab, we drew a diagram on the board that showed how the hashmap consisted of an array ["buckets"] of linked-lists. Once I was able to understand what the program was asking of me conceptually, I was able to implement the hashmap and linked-lists based on my prior knowledge from CS 1112, combined with knowledge of C from this course. The one method that I needed the most help on was the `resize()` method, because it just required lots of in-depth discussion about pointers, memory, and how to correctly resize the hashmap and copy all of the elements over. That method was where I asked the most questions and received the input from TA's.

After getting through (and spending lots of time debugging) `resize()`, I found it significantly easier to code the Boggle portion of the lab. I found that once I had cleared all of my `llist_t` and `hashmap_t` methods of errors, it was simple to implement them in the Boggle file. For the first part (opening the dictionary file and adding all of the words to the hashmap) I just referened notes from earlier lessons about using `fscanf` to read in the `.txt` file. For the second and third parts of the project, I referenced some notes from CS1112 about recursion and base cases, which was a helpful refresher. Implementing those methods in C was mainly a matter of trial and error, but the debugger was helpful in tracking down errors and working through any problems.
