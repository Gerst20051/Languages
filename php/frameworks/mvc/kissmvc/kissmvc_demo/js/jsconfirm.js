function jsconfirm(msg, url) {
  var ans = confirm(msg);
  if (ans)
    window.location = url;
}