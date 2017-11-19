package Course;
import java.util.ArrayList;
import java.util.List;

//备选课程类
public class ListTest {

    //用于存放备选课程的容器
    private List coursesToSelecet;
    private int num ;

    public ListTest() {
        this.coursesToSelecet = new ArrayList();
        this.num = 0 ;
    }

    //用来向容器添加备选课程的方法
    public void testAdd() {
        course cr1 = new course("1", "数据结构");
        coursesToSelecet.add(cr1);
        num ++ ;
        //从集合中取出来的元素需要进行强制转换
        course temp = (course) coursesToSelecet.get(num - 1);
        System.out.println("添加了课程：" + temp.id + ":" + temp.name);
    }

    public static void main(String[] args) {
        ListTest it = new ListTest();
        it.testAdd();
    }
}
