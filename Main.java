////抽象方法只会声明在abstract 关键字的类中
//abstract class Person {
//    private String name;
//    public String getName() {
//        return this.name;
//    }
//
//    public void setName() {
//        this.name = name;
//    }
//
//    //抽象方法不能够进行实例化
//    public abstract void getInfo();
//}
//
//class Student extends Person{
//    public void getInfo() {
//        System.out.println("I am a student");
//    }
//}
//
//interface IMessage {
//    abstract void print();
//}
//
//class MessageImpl implements IMessage {
//    public void print() {
//        System.out.println("I am a biter");
//    }
//}
//
//public class Main {
//    public static void main(String[] args) {
//        //多态的特性
//        MessageImpl mes = new MessageImpl();
//        mes.print();
//    }
//}


//interface IMessage {
//    public void print();
//}
//
//abstract class News implements IMessage {
//    public abstract void getNews();
//}
//
//class MessageImpl extends News {
//    public void print() {
//        System.out.println("I am a biter");
//    }
//
//    public void getNews() {
//        System.out.println("I am a news");
//    }
//}
//
//public class Main{
//    public static void main(String[] args) {
//        IMessage m = new MessageImpl();
//        m.print();
//    }
//}


////设计模式--工厂模式
//
//import com.sun.istack.internal.NotNull;
//
//interface IFruit {
//    public void eat();
//}
//
//class Apple implements IFruit {
//    public void eat() {
//        System.out.println("eat apple");
//    }
//}
//
//class Melon implements IFruit {
//    public void eat() {
//        System.out.println("eat melon");
//    }
//}
//
//class Factory {
//    public static IFruit getInstance(String className) {
//        if("apple".equals(className)) {
//            return new Apple();
//        }
//
//        if("melon".equals(className)) {
//            return new Melon();
//        }
//
//        return null;
//    }
//}
//
//
//public class Main {
//    public static void main(String[] args){
//        IFruit p = Factory.getInstance("apple");
//        if(p!=null)
//            p.eat();
//    }
//}

//public class Main {
//    public static void main(String[] args) {
////        String str1 = "Hello";
////        //直接赋值则直接进行引用
////        String str2 = new String("Hello").intern();
////        //用在对象上面表示的是对象的地址
////        System.out.println(str1==str2);
////        System.out.println(str1.equals(str2));
////        String str = "lebron james";
////        char[] charArray = str.toCharArray();
////        for(int i=0;i<charArray.length; i++) {
////            if(charArray[i]!=' ')
////                charArray[i] -= 32;
////        }
////        System.out.println(new String(charArray));
//
////        String name = new String("Lebron James");
////        System.out.println(name.startsWith("Lebron "));
//
//        //ip地址的拆分
////        String ipName = "192.168.220.17";
////        String[] res = ipName.split("\\.");
////        for (String result: res
////             ) {
////            System.out.println(result);
////        }
//    }
//}

//class SingleTon {
//    public static SingleTon insTance = new SingleTon();
//    private SingleTon() {}
//
//    public void print() {
//        System.out.println("Hello ,World");
//    }
//}
//
//public class Main {
//    public static void main(String[] args) {
//        SingleTon single = null;
//        single = SingleTon.insTance;
//        single.print();
//    }
//}

import java.util.LinkedList;

//链表的实现
class classNode<T> {
    public T data;
    public classNode next;
    public classNode front;
    public classNode(){
        this.next = null ;
        this.front = null ;
    }

    public void setData(T data) {
        this.data = data ;
    }

    public void setNext(classNode Node) {
        this.next = Node ;
    }
}

class linkList<T> {
    private classNode _begin;
    private classNode _end;
    private int size ;
    public linkList() {
        this._begin = new classNode();
        this._end = new classNode();
        _begin.next = _end;
        _end.front = _begin;
        size = 0;
    }

    public void pushBack(T data) {
        classNode<T> newNode = new classNode<>();
        newNode.setData(data);
        classNode<T> frontNode = _end.front;

        frontNode.next = newNode;
        newNode.next = _end;
        newNode.front = frontNode;
        _end.front = newNode;
        size++;
    }

    public int getSize() {
        return this.size;
    }

    public void pushFront(T data) {
        classNode<T> newNode = new classNode<>();
        newNode.setData(data);

        newNode.next = _begin.next;
        _begin.next.front = newNode;
        newNode.front = _begin;
        _begin.next = newNode;
    }

    public void printList() {
        classNode<T> cur = _begin.next;
        while(cur!=null && cur!= _end) {
            System.out.println(cur.data);
            cur = cur.next;
        }
    }

    public boolean delNode(T data) {
        classNode<T> cur = _begin.next ;
        while(cur!=null) {
            if(cur.data.equals(data)) {
                break;
            }

            cur = cur.next ;
        }

        if(cur!=null){
            classNode<T> frontNode = cur.front ;
            frontNode.next = cur.next;
            cur.next.front = frontNode;
            return true;
        }
        return false;
    }
}

public class Main {
    public static void main (String[] args) {
        linkList<Integer> link = new linkList<>();
        link.pushBack(1);
        link.pushBack(2);
        link.pushBack(3);
        link.pushFront(4);
        link.pushFront(5);

        link.printList();
        link.delNode(3);
        link.printList();
    }
}