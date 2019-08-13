<?php    
include "connect.php";
?>

<!doctype html>
<head>
<meta charset="UTF-8">
<title>게시판</title>
<link rel="stylesheet" type="text/css" href="/css/style.css" />
</head>
<body>
<div id="board_area"> 
  <h1>사진게시판</h1>
  <h4>사진 보관소.</h4>
    <table class="list-table">
      <thead>
          <tr>
                <th width="70">번호</th>
                <th width="500">제목</th>
                <th width="500">URL</th>
                <th width="100">크기</th>
            </tr>
        </thead>

          <?php
          $query = "SELECT * FROM picture";
          $sql = mysqli_query($connect, $query);
          $total = mysqli_num_rows($sql);
          $pageNum = ($_GET['page']) ? $_GET['page'] : 1;
          $list = ($_GET['list']) ? $_GET['list'] : 5;
          $limit = ($pageNum -1) * $list;
          $query = "SELECT * FROM picture order by id desc limit $limit, $list";
          $sql = mysqli_query($connect, $query);
          
          while($data = mysqli_fetch_array($sql))
          {
              $title=$data["FileName"]; 

              if(strlen($title)>30)
              { 
                  $title=str_replace($data["FileName"],mb_substr($data["FileName"],0,30,"utf-8")."...",$data["FileName"]); //title이 30을 넘어서면 ...표시
              }
?> 

      <tbody>
        <tr>
          <td width="70"><?php echo $data['id'] ?></td>
          <td witdh="500"><a href="/read.php?id=<?php echo $data["id"]; ?>"><?php echo $title;?></a></td>
          <td width="500"><?php echo $data['imgURL'] ?></td>
          <td width="100"><?php echo $data['Size'] ?></td>
        </tr>
      </tbody>

      <?php }
?>
          </table>
    <div id="page_num">
        <tr> 
            <td height="30" align="center" valign="middle" colspan="50" style="border:1px #CCCCCC solid;"> 
        <?            
              $b_pageNum_list = 10; //블럭에 나타낼 페이지 번호 갯수 
              $block = ceil($pageNum / $b_pageNum_list); //현재 리스트의 블럭 구하기 

              $b_start_page = (($block - 1) * $b_pageNum_list ) + 1; //현재 블럭에서 시작페이지 번호
              $b_end_page = $b_start_page + $b_pageNum_list - 1; //현재 블럭에서 마지막 페이지 번호 

              $total_page = ceil($total/$list); //총 페이지 수 
              $size = 3;

              if($total_page < $b_end_page) 
                 $b_end_page = $total_page; 

              if($pageNum <= 1){?> 
                <font size=3 color=red>처음</font> 
              <?}
              else{?> 
                  <font size=3><a href="/table.php?page=&list=<?=$list?>">처음</a></font>
              <?} 

              if($block <= 1){?> 
                <font> </font> 
              <?}
              else{?> 
                  <font size=3><a href="/table.php?page=<?=$b_start_page-1?>&list=<?=$list?>">이전</a></font>
              <?} 

              for($j = $b_start_page; $j <= $b_end_page; $j++) 
              { 
                if($pageNum == $j) 
                {?> 
                    <font size=3 color=red><?=$j?></font> 
                <?}
                else{?> 
                    <font size=3><a href="/table.php?page=<?=$j?>&list=<?=$list?>"><?=$j?></a></font>
                <?}
              } 

              $total_block = ceil($total_page/$b_pageNum_list); 

              if($total_block <= $block)
              {?> 
                <font> </font> 
              <?}
              else
              {?> 
                <font size=3><a href="/table.php?page=<?=$b_end_page+1?>&list=<?=$list?>">다음</a></font>
              <?} 

              if($total_page <= $pageNum)
              {?> 
                <font size=3 color=red>마지막</font> 
              <?}
              else
              {?> 
                <font size=3><a href="/table.php?page=<?=$total_page?>&list=<?=$list?>">마지막</a></font>
            <?}
            mysql_close($connect);
            ?> 
             </td> 
    </tr>
  </div>
</body>
</html>

