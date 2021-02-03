<?php

/**
 * 自然排序函数
 * @param array $list 一维数组
 * @return array 排序后的一维数组
 */
function nature_sorted($list) {

    /**
     * 回调：比较函数
     */
    function mycmp($a, $b) {
        $collator = new Collator("zh-CN");
        return $collator->compare($a, $b);
    }

    $db = new SQLite3(":memory:");
    $db->exec("CREATE TABLE test (col1 string)");

    foreach ($list as $one) {
        $db->exec("INSERT INTO test VALUES ('{$one}')");
    }

    $db->createCollation('NATURAL_CMP', 'mycmp');
    $naturalSort = $db->query("SELECT col1 FROM test ORDER BY col1 COLLATE NATURAL_CMP");

    $ret = array();
    while ($row = $naturalSort->fetchArray()) {
        $ret[] = $row['col1'];
    }

    $db->close();

    return $ret;
}

/**
 * 测试用例入库函数
 * @param type $argc
 * @param type $argv
 */

function main($argc, $argv) {
    $l = array("张三", "李四", "王五", "a1", "a21", "b2", "12", "10");
    echo "原始排序：\n";
    foreach ($l as $i) {
        echo $i . "\n";
    }

    echo "自然排序：\n";
    $r = nature_sorted($l);
    foreach ($r as $i) {
        echo $i . "\n";
    }
}

main($argc, $argv);