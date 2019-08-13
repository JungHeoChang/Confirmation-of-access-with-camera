<!doctype html>
<head>
<meta charset="UTF-8">
<title>사진</title>
<link rel="stylesheet" type="text/css" href="/css/style.css" />
</head>
<body>
<?php
        
        $conn = mysqli_connect('localhost', 'root', 'jung!@', 'Ai214');
        $bno = $_GET['id'];
        $query = "SELECT * FROM picture where id='".$bno."'";
        $result = mysqli_query($conn, $query);
        //$sql = "SELECT * FROM php_pic where id='".$bno."'");

        $data = mysqli_fetch_array($result);
		$prevPage = $_SERVER["HTTP_REFERER"];
        /*
        $bno = $_GET['idx'];
        $sql = mq("select * from php_pic where id='".$bno."'");
        $data = $sql -> fetch_array();
        //$path = "/upload/";(
        */
?>

<div id="board_read">
    <h1>사진 게시판</h1>
    <h2><td width="200"><?php echo $data['FileName'] ?></td></h2>
        <div id="pic_info">
            <?php echo '<img src='.$data['imgURL'].'><br>'?>
        </div>
        <div id="back_btn">
            <a href="<?=$prevPage?>"><button>뒤로가기</button></a>
</div>
</body>
</html>


