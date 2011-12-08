<!--Connect to database-->
<?php include("./dbconnect.php"); ?>

<?php 
//$username="root";
//$password="";
//$database="cssjsuor_cs160s2011g2";
//$dblocal="localhost";

//mysql_connect($dblocal,$username,$password);
//@mysql_select_db($database) or die("Unable to select database");

//get recommendation posted and last update date from fulfilled recommendation
//$query = "SELECT node.title AS node_title, node.nid AS nid, node.created AS node_created," .
//	" 'Fulfilled' AS field_data_field_letter_status_node_entity_type, GREATEST(node.changed, node_comment_statistics.last_comment_timestamp)" .
//	" AS node_comment_statistics_last_updated FROM node" .
//	" INNER JOIN field_data_field_letter_status ON node.nid = field_data_field_letter_status.entity_id" .
//	" AND (field_data_field_letter_status.entity_type = 'node' AND field_data_field_letter_status.deleted = '0')" .
//	" INNER JOIN node_comment_statistics ON node.nid = node_comment_statistics.nid" .
//	" WHERE (( (node.status = '1') AND (node.type IN  ('recommendation_request'))" .
//	" AND (field_data_field_letter_status.field_letter_status_tid = '426') ))" .
//        " ORDER BY node_created DESC LIMIT 10 OFFSET 0";
$query = "SELECT node.title AS node_title, node.nid AS nid, node.created AS node_created," .
	" 'Fulfilled' AS field_data_field_letter_status_node_entity_type, GREATEST(node.changed, node_comment_statistics.last_comment_timestamp)" .
	" AS node_comment_statistics_last_updated FROM node" .
	" INNER JOIN field_data_field_letter_status ON node.nid = field_data_field_letter_status.entity_id" .
	" AND (field_data_field_letter_status.entity_type = 'node' AND field_data_field_letter_status.deleted = '0')" .
	" INNER JOIN node_comment_statistics ON node.nid = node_comment_statistics.nid" .
	" WHERE (( (node.status = '1') AND (node.type IN  ('recommendation_request'))" .
	" AND (field_data_field_letter_status.field_letter_status_tid = '426') ))" .
        " ORDER BY node_created DESC LIMIT 10 OFFSET 0";
//print($query);
$result = mysql_query($query);
//check if current input already exist
if (!$result) 
{
    $message  = 'Error: ' . mysql_error() . "\n";
	die($message);
}
elseif ($result)
{
	$count = 0;
	$dur_total = 0;
	print "<table border='1'>";
	print "<tr>";
	print "<td>Title</td>";
	print "<td>Node ID</td>";
	print "<td>Post date</td>";
	print "<td>Status</td>";
	print "<td>Last update</td>";
	print "<td>Duration(Days)</td>";
	print "</tr>";
	//print "Title\tNode ID\tPost date\tStatus\tLast update\tduration";
	//print "<br>\n";
	while (list($title, $nid, $post, $status, $last_update) = mysql_fetch_array($result))
	{
		print "<tr>";
		$duration = strtotime($post) - strtotime($last_update);
		$post = date('Y-m-d h:i:s', $post);
		$last_update = date('Y-m-d h:i:s', $last_update);
		//print "$title, $nid, $post, $status, $last_update, $duration days";
		//print "<br>\n";
		print "<td>$title</td>";
		print "<td>$nid</td>";
		print "<td>$post</td>";
		print "<td>$status</td>";
		print "<td>$last_update</td>";
		print "<td>$duration</td>";
		$count++;
		$dur_total = $dur_total + $duration;
		print "</tr>";
	}
	print "</table>" ;
	$avg_total = $dur_total / $count;
	print "<br>\n";
	print "<b>Avg. duration to fulfilled a recomendation request = $avg_total</b>";
	print "<br>\n";
}

?>
<!-- close database connection? -->
<?php mysql_close(); ?>