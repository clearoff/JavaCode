import java.lang.reflect.*;
import java.util.ArrayList;

////Foo 也是对象是Class类的对象
////这个对象我们成为类类型
//class Foo {
//    public static void print() {
//        System.out.println("this is foo ... ");
//    }
//}
//
//public class Main {
//    public static void main (String[] args){
//        Class c1 = Foo.class;
//        Foo foo1 = new Foo() ;
//        Class c2 = foo1.getClass();
//        Class c3;
//        try{
//            c3 = Class.forName("Foo");
//            //
//        }catch (ClassNotFoundException e){
//            e.printStackTrace();
//        }
//
//        //通过类类型创建对象实例---> newInstance()
//        //需要有无参数的构造方法
//        try {
//            Foo foo2 = (Foo) c1.newInstance();
//            foo2.print();
//        }catch (InstantiationException e){
//            e.printStackTrace();
//        }catch (IllegalAccessException e){
//            e.printStackTrace();
//        }
//
//    }
//}

//class Calc {
//    public static void Add(int num1 , int num2 ){
//        System.out.print(num1 + "+" + num2 + "=" + (num1+num2) );
//    }
//}
//
//public class Main {
//    public static void main(String[] args) {
//        Calc clc = new Calc();
//        Class cls = clc.getClass();
//        try{
//            Method m1 = cls.getMethod("Add",int.class,int.class);
//            //method reflect operation
//            m1.invoke(clc,new Object[]{10,20});
//        }catch(Exception e){
//            e.printStackTrace();
//        }
//    }
//}


public class Main {
    public static void main(String[] args) {
        ArrayList list = new ArrayList();
        ArrayList<String> list1 = new ArrayList<String>();
        list1.add("Hello, World");
        Class c1 = list.getClass();
        Class c2 = list1.getClass();
//        if(c1 == c2) {
//            System.out.println("去擦除");
//        }
//        else{
//            System.out.println("not");
//        }
        try {
            Method m1 = c2.getMethod("add",Object.class);
            m1.invoke(list1, 100);
            System.out.println(list1.size());
            for(int i=0; i<list1.size() ; i++) {
                System.out.println(list1.get(i));
            }
        }catch (Exception e){
            e.printStackTrace();
        }
    }
}