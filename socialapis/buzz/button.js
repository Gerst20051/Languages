// http://www.google.com/buzz/post?message=Here's%20Buzz!&url=http://www.google.com/buzz
// http://www.google.com/buzz/stuff
// http://www.google.com/buzz/post?url=<URL to post>&imageurl=<Optional image URL>

window.open(
  "http://www.google.com/buzz/post?" +
  "message=Here's%20Buzz!&url=http://www.google.com/buzz",
  "_blank",
  "resizable=0,scrollbars=0,width=690,height=415"
);