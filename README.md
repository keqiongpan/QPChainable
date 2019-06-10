# QPChainable

[![GitHub License](https://img.shields.io/github/license/keqiongpan/QPChainable.svg)](https://github.com/keqiongpan/QPChainable/blob/master/LICENSE)
[![CocoaPods Version](https://img.shields.io/cocoapods/v/QPChainable.svg)](https://cocoapods.org/pods/QPChainable)
[![Carthage Compatible](https://img.shields.io/badge/carthage-compatible-4BC51D.svg?style=flat)](https://github.com/Carthage/Carthage)
[![CocoaPods Platforms](https://img.shields.io/cocoapods/p/QPChainable.svg)](https://cocoapods.org/pods/QPChainable)

为Objective-C提供便捷的方法构造支持链式表达式的功能。


## 使用CocoaPods集成

QPChainable 支持使用第三方库管理工具 [CocoaPods](http://cocoapods.org) 集成到你的 Xcode 工程。详情可查看 [这里](https://cocoapods.org/pods/QPChainable) 。

可以在 Podfile 文件中添加如下项：

```
pod 'QPChainable', '~> 1.0'
```


## 使用Carthage集成

QPChainable 同时兼容使用去中心化的库管理工具 [Carthage](https://github.com/Carthage/Carthage)  集成到你的 Xcode 工程。

可以在 Cartfile 文件中添加如下项：

```
github "keqiongpan/QPChainable" ~> 1.0
```


## 使用说明

### 1. 声明链式表达式函数

为某个类的添加支持的链式表达式函数，可以在该类的声明体内使用 `QP_CHAINABLE_DECLARE` 函数宏声明该函数。

使用方法：

```Objective-C
QP_CHAINABLE_DECLARE(<函数名>, <参数列表>[, <返回值类型>]);
```

其中 `<参数列表>` 需要使用圆括号 `()` 括起来； `<返回值类型>` 则为可选项，默认为 instancetype 。

使用示例：

```Objective-C
@interface QPPacketMaker : NSObject

- QP_CHAINABLE_DECLARE(length, (void), NSUInteger);
- QP_CHAINABLE_DECLARE(append, (QPMultipartData *data), QPPacketMaker *);
- QP_CHAINABLE_DECLARE(send, (void), QPPacketMaker *);

@end
```


### 2. 实现链式表达式函数

当声明了类支持的链式表达式函数，可以在该类的实现体内使用 `QP_CHAINABE_IMPLEMENTATION` 函数宏定义函数的实现逻辑。

使用方法：

```Objective-C
QP_CHAINABLE_IMPLEMENTATION(<函数名>, <参数列表>[, <返回值类型>], <函数体>);
```

其中 `<参数列表>` 需要使用圆括号 `()` 括起来； `<返回值类型>` 则为可选项，默认为 instancetype ； `<函数体>` 需要使用 `({` 和 `})` 括起来。

使用示例：

```Objective-C
@implementation QPPacketMaker

#pragma mark length()
- QP_CHAINABLE_IMPLEMENTATION(length, (void), NSUInteger, ({
    return self.data.length;
}))

@end
```


### 3. 支持继承混用的链式表达式函数

如果不做特殊处理的情况下，父类与子类分别实现的链式表达式函数是不能进行混用的，因为这些函数的返回值类型是不一致的，父类的链式表达式函数返回的一般是父类实例对象类型，无法通过点语法“.”调用子类的链式表达式函数。

要想实现父类与子类实现的链式表达式函数交叉混用，可以将父类和子类都改造为泛型类，并将最终使用的实例对象类型通过泛型参数传递给父类和子类，两者均使用该类型作为链式表达式函数的返回值类型。另外还要多实现一个装饰类，用于提供给调用者最终使用。

装饰类的声明与实现，可以使用下面这对函数宏：

```Objective-C
QP_CHAINABLE_DECORATOR_DECLARE(<装饰类>, <具体类>);
QP_CHAINABLE_DECORATOR_IMPLEMENTATION(<装饰类>, <具体类>);
```

其中 `<具体类>` 是 `<装饰类>` 的父类，是带一个实例类型参数的泛型类，而 `<装饰类>` 是普通类，没有泛型参数。

宏展开时会为 `<具体类>` 添加一个 `- (<装饰类> *)make` 的方法，使得无论当前对象的类型是装饰类还是其父类的实例对象类型，都可以通过调用 make 来启动链式表达式。

使用示例：

```Objective-C
@interface QPConcretePacketMaker<instancetype> : NSObject

- QP_CHAINABLE_DECLARE(length, (void), NSUInteger);
- QP_CHAINABLE_DECLARE(append, (QPMultipartData *data));
- QP_CHAINABLE_DECLARE(send, (void));

@end

@interface QPConcreteVideoPacketMaker<instancetype> : QPConcretePacketMaker<instancetype>

- QP_CHAINABLE_DECLARE(subtitle, (QPSubtitleData *data));

@end

QP_CHAINABLE_DECORATOR_DECLARE(QPVideoPacketMaker, QPConcreteVideoPacketMaker);

//
// 调用示例：
// QPVideoPacketMaker *maker = [[QPVideoPacketMaker alloc] init];
// maker.append(videoData).subtitle(subtitleData).send();
//
```


### 4. 支持数组字面量的链式表达式函数

如果一个链式表达式函数需要传入一个数组对象（NSArray \*），可以有多种方法，如数组对象或数组字面量（@[<对象1>, ...]）等。但这些方法都不够优美，数组对象需要另外在之前拼装好数组内容，数组字面量在折行对齐时，前面很有可能会留有很大一块空白。

现在，可以使用如下函数宏来定义链式表达式函数，以提供更优美的方法传递数组字面量：

```Objective-C
QPChainableArray(<对象类型>, {<对象1>, ...})
```

使用示例：

```Objective-C
#define members(...) \
        membersFromArray(QPChainableArray(NSString *, __VA_ARGS__))
- QP_CHAINABLE_DECLARE(members, (void *_FOR_CODE_HINT_ONLY_));
- QP_CHAINABLE_DECLARE(membersFromArray, (NSArray<NSString *> *));

unsc.permanent.members({
    @"中国",
    @"美国",
    @"英国",
    @"法国",
    @"俄罗斯"
})
```


### 5. 支持字典字面量的链式表达式函数

如果一个链式表达式函数需要传入一个字典对象（NSDictionary \*），同样可以有多种方法，如字典对象或字典字面量（@{<键1>:<值1>, ...]）等。但这些方法都不够优美，字典对象需要另外在之前拼装好字典内容，字典字面量在折行对齐时，前面很有可能会留有很大一块空白。

现在，可以使用如下函数宏来定义链式表达式函数，以提供更优美的方法传递字典字面量：

```Objective-C
QPChainableDictionary(<键类型>, <值类型>, {<键1>:<值1>, ...})
```

使用示例：

```Objective-C
#define members(...) \
        membersFromDictionary(QPChainableDictionary(NSString *, NSString *, __VA_ARGS__))
- QP_CHAINABLE_DECLARE(members, (void *_FOR_CODE_HINT_ONLY_));
- QP_CHAINABLE_DECLARE(membersFromDictionary, (NSDictionary<NSString *, NSString *> *));

unsc.permanent.members({
    @"CN" : @"中国",
    @"US" : @"美国",
    @"UK" : @"英国",
    @"FR" : @"法国",
    @"RU" : @"俄罗斯"
})
```
