package com.demo.springmvc.controller;

import com.demo.springmvc.entity.User;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestMapping;

@Controller
@RequestMapping(value="/user3")
public class PathValueController {

    /*
        PathVariable:用来匹配url中的参数
     */
    @RequestMapping(value="/register/{userName}/{userPassword}")
    public String register(@PathVariable("userName")String userName,
                           @PathVariable("userPassword")String userPassword,
                           Model model) {
        User user = new User();
        user.setUsername(userName);
        user.setPassword(userPassword);

        model.addAttribute("user",user);
        return "success";
    }
}
