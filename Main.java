
//class myThread implements Runnable{
//    private int tickets = 10;
//
////    @Override
////    public void run(){
////       for(int i=0;i<1000;i++){
////           //同步代码块
////           synchronized (this){
////               if(this.tickets > 0){
////                   try{
////                       Thread.sleep(200);
////                   }
////                   catch (Exception e){
////                       e.printStackTrace();
////                   }
////                   System.out.println(Thread.currentThread().getName()+"还剩"+tickets--+"票");
////               }
////           }
////       }
////    }
//    public void run(){
//        for(int i=0; i<1000;i++){
//            this.sale();
//        }
//    }
//
//    private synchronized void sale(){
//        if(this.tickets > 0){
//            try {
//                Thread.sleep(200);
//            }catch (Exception e){
//                e.printStackTrace();
//            }
//            System.out.println(Thread.currentThread().getName()+"还剩"+tickets--+"票");
//        }
//    }
//}
//
//public class Main {
//    public static void main(String[] args){
//        myThread mt = new myThread();
//        new Thread(mt,"threadA").start();
//        new Thread(mt,"threadB").start();
//    }
//}

//class Sync {
//    private String word = "Hello, Java";
//    public void test() {
//        synchronized (this) {
//            System.out.println("test begin ..." + Thread.currentThread().getName() + " " + this.word);
//            try {
//                Thread.sleep(200);
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//            System.out.println("test end ..." + Thread.currentThread().getName());
//        }
//    }
//}


//class myThread implements Runnable {
//    private Sync sync;
//    public myThread(Sync sync){
//        this.sync = sync;
//    }
//
//    public void run(){
//        this.sync.test();
//    }
//}
//
//public class Main {
//    public static void main(String[] args){
//        Sync sync = new Sync();
//
//        for(int i=0;i<3 ;i++){
//            myThread mt = new myThread(sync);
//            new Thread(mt,"thread"+i).start();
//        }
//    }
//}

//使用全局锁
//class Sync {
//    public void test(){
//        synchronized (Sync.class){
//            System.out.println("test begin ..." + Thread.currentThread().getName());
//            try {
//                Thread.sleep(200);
//            } catch (Exception e) {
//                e.printStackTrace();
//            }
//
//            System.out.println("test end ..." + Thread.currentThread().getName());
//        }
//    }
//}
//
//class myThread implements Runnable  {
//    public void run(){
//        new Sync().test();
//    }
//}
//
//public class Main {
//    public static void main(String[] args){
//        myThread mt = new myThread();
//        for(int i=0;i<10;i++){
//            new Thread(mt,"thread "+i).start();
//        }
//    }
//}

//import java.util.concurrent.SynchronousQueue;
//
//class Pen {
//    private String pen = "this is a pen";
//
//    public String getPen() {
//        return pen;
//    }
//}
//
//class Book {
//    private  String book = "this is a book";
//
//    public String getBook(){
//        return book;
//    }
//}
//
//public class Main{
//    private static Pen pencil = new Pen();
//    private static Book newBook = new Book();
//
//    public void deadLock() {
//        Thread thread1 = new Thread(new Runnable(){
//            @Override
//            public void run() {
//                synchronized (pencil){
//                    System.out.println(Thread.currentThread().getName() + " 我有笔，我就不给你");
//
//                    synchronized (newBook){
//                        System.out.println(Thread.currentThread().getName()+ " 把你的书给我");
//                    }
//                }
//            }
//        },"pen");
//
//        Thread thread2 = new Thread(new Runnable() {
//            @Override
//            public void run() {
//                synchronized (newBook){
//                    System.out.println(Thread.currentThread().getName()+"我有书，我就不给你");
//
//                    synchronized (pencil){
//                        System.out.println(Thread.currentThread().getName()+"把你的笔借我用用");
//                    }
//                }
//            }
//        },"book");
//        thread1.start();
//        thread2.start();
//    }
//    public static void main(String[] args) {
//        new Main().deadLock();
//    }
//}

//生产者消费者模型（不带条件变量和锁资源）
//class Data {
//    private String title ;
//    private String note ;
//
//    public void setTitle(String title){
//        this.title = title ;
//    }
//
//    public String getTitle() {
//        return title ;
//    }
//
//    public void setNote(String note){
//        this.note = note ;
//    }
//
//    public String getNote() {
//        return note;
//    }
//}
//
//class DataConsumer implements Runnable {
//    private Data data;
//
//    public DataConsumer(Data data) {
//        super();
//        this.data = data;
//    }
//
//    @Override
//    public void run(){
//        for(int i=0; i<50; i++){
//            try{
//                Thread.sleep(500);
//            }catch (InterruptedException e){
//                e.printStackTrace();
//            }
//            System.out.println(this.data.getTitle()+"##"+this.data.getNote());
//        }
//    }
//}
//
//class DataProvider implements Runnable {
//    private Data data;
//    public DataProvider(Data data){
//        super();
//        this.data = data ;
//    }
//
//    @Override
//    public void run() {
//        for(int i=0; i<50; i++){
//            try{
//                Thread.sleep(500);
//            }catch (Exception e){
//                e.printStackTrace();
//            }
//            if(i%2 == 0){
//                this.data.setTitle("lebron-james");
//                this.data.setNote("联盟第一人");
//            }else{
//                this.data.setTitle("kevin-durant");
//                this.data.setNote("千年老二");
//            }
//        }
//    }
//}
//
//public class Main {
//    public static void main(String[] args) {
//        Data data = new Data();
//        new Thread(new DataConsumer(data)).start();
//        new Thread(new DataProvider(data)).start();
//    }
//}

class Data {
    private String title ;
    private String note ;
    private boolean flag ;
    //flag = true :表示允许生产,不允许消费
    //flag = false :表示生产完毕,可以消费
    public synchronized void set(String title, String node){
        if(this.flag == false){
            try{
                super.wait();
            }catch(InterruptedException e){
                e.printStackTrace();
            }
        }
        this.title = title ;
        try {
            Thread.sleep(10);
        }catch (InterruptedException e){
            e.printStackTrace();
        }
        this.note = note;
        this.flag = false ;
        super.notify();
    }

    public synchronized void get(){
        if(this.flag == true ){
            try{
                super.wait();
            }catch(InterruptedException e){
                e.printStackTrace();
            }
        }
        try{
            Thread.sleep(20);
        }catch (InterruptedException e){
            e.printStackTrace();
        }
        System.out.println(this.title + "###"+this.note);
        this.flag = true;
        super.notify();
    }
}

class DataProvider implements Runnable {
    private Data data;
    public DataProvider(Data data){
        super();
        this.data = data ;
    }

    @Override
    public void run(){
        for(int i=0;i<20;i++){
            if(i%2 == 0){
                this.data.set("lebron-james","联盟第一人");
            }else{
                this.data.set("kevin-durant","千年老二");
            }
        }
    }
}

class DataConsumer implements Runnable {
    private Data data;
    public DataConsumer(Data data){
        super();
        this.data = data;
    }

    @Override
    public void run(){
        for(int i=0; i<50; i++){
            try{
                Thread.sleep(500);
            }catch (InterruptedException e){
                e.printStackTrace();
            }
            this.data.get();
        }
    }
}

public class Main {
    public static void main(String[] args) {
        Data data = new Data();
        new Thread(new DataProvider(data)).start();
        new Thread(new DataConsumer(data)).start();
    }
}