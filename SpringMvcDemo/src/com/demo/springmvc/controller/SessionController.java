package com.demo.springmvc.controller;

import com.demo.springmvc.entity.User;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.SessionAttributes;

@Controller
@RequestMapping(value = "/user1")
@SessionAttributes(value="user",types = {User.class})
public class SessionController {

    @RequestMapping(value = "/register")
    public String register(@ModelAttribute("form")User user,Model model){
        model.addAttribute("user",user);
        return "success";
    }
}
