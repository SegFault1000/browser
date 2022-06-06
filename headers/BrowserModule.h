#pragma once
struct BrowserModule
{
  virtual void Init(class Widget*) = 0;
  BrowserModule() = default;
  virtual ~BrowserModule() = default;

  BrowserModule(const BrowserModule&) = delete;
  BrowserModule(BrowserModule&&) = delete;
};