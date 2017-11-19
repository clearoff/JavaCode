package Course;
import java.util.HashSet;
import java.util.Set;

public class student {
    public String id ;
    public String name ;
    public Set courses;

    public student(String id , String name) {
        this.id = id ;
        this.name = name ;
        this.courses = new HashSet() ;
    }
}
