<script>
function showForbidden()
{
header('HTTP/1.1 403 Forbidden');

echo("<html>
<head>
<title>403 Forbidden</title>
</head>
<body>
<p>Directory access is forbidden.</p>
</body>
</html>");
}
</script>