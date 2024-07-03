// Problem Statement 3: Explain the code snippet

// Explain what the following code is attempting to do? You can explain by:
// Explaining how the highlighted constructs work?
// Giving use-cases of what these constructs could be used for.
// What is the significance of the for loop with 4 iterations?
// What is the significance of make(chan func(), 10)?
// Why is “HERE1” not getting printed?

package main

import "fmt"

func main() {
	cnp := make(chan func(), 10)
	for i := 0; i < 4; i++ {
		go func() {
			for f := range cnp {
				f()
			}
		}()
	}
	cnp <- func() {
		fmt.Println("HERE1")
	}
	fmt.Println("Hello")
}

// What is the significance of make(chan func(), 10)?
=> Channel Creation (make(chan func(), 10)): 
 make(chan func(), 10): This creates a buffered channel cnp that can hold up to 10 elements. The elements in this channel are functions with no parameters and no return values.



// Giving use-cases of what these constructs could be used for.
1. Buffered Channels: Useful for decoupling the sender and receiver in concurrent programs. The buffer allows the sender to proceed without waiting for the receiver to be ready.
2. Goroutines: Lightweight threads managed by the Go runtime. They allow for concurrent execution of functions.
3. Channel Ranges: Useful for reading from a channel until it is closed. In this case, the goroutines will continuously read from cnp until the channel is closed.

// What is the significance of the for loop with 4 iterations?
=> For Loop with 4 Iterations:

    for i := 0; i < 4; i++: This loop runs 4 times, creating 4 separate goroutines.
    go func() { for f := range cnp { f() } }(): Inside the loop, an anonymous goroutine is created. Each goroutine continuously reads from the cnp channel and executes the functions received from the channel.



// Why is “HERE1” not getting printed?


	=>Channel Send Operation (cnp <- func() { fmt.Println("HERE1") }):

    This line sends a function that prints "HERE1" into the cnp channel.

    =>Print Statement (fmt.Println("Hello")):

    This line prints "Hello" to the standard output.