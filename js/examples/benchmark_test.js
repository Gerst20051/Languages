var iterations = 100;
var totalTime = 0;
 
// Record the starting time, in UTC milliseconds.
var start = new Date().getTime();
 
// Repeat the test the specified number of iterations.
for (i = 0; i &lt; iterations; i++) {
  // Execute the selector. The result does not need
  //  to be used or assigned to determine how long 
  //  the selector itself takes to run.
  $(&#039;[id$=comment-35496]&#039;);
}
 
// Record the ending time, in UTC milliseconds.
var end = new Date().getTime();
 
// Determine how many milliseconds elapsed
totalTime = (end - start); 
 
// Report the average time taken by one iteration.
alert(totalTime / iterations);