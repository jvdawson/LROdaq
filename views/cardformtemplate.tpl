<html>
<head>
<title>title</title>
</head>

<body>


<p> There are {{connections}} connections </p>
 <form action="/init" method="post">

% for num, addrip in enumerate(ip, start=0):
  IP address: <input type ="text" name={{'ip%s' % (num)}} value={{addrip}} /> <input type="checkbox" name={{'check%s' % (num)}} /> </p>



% end

<input value="Go!" type="submit" />
 
</form>


</body>


</html>
