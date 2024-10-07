const { expect } = require("chai");

describe("MusicToken", function () {
  let Token;
  let MusicToken;
  let owner;
  let addr1;
  let addr2;
  let totalSupply = 100n;

  beforeEach(async function () {
    Token = await ethers.getContractFactory("MusicToken");
    [owner, addr1, addr2] = await ethers.getSigners();
    MusicToken = await Token.deploy(totalSupply);
  });

  it("トークンの名前とシンボルが正しいことを確認する", async function () {
    expect(await MusicToken.name()).to.equal("MusicToken");
    expect(await MusicToken.symbol()).to.equal("MT");
  });

  it("コントラクトのデプロイ後、オーナーに初期供給が割り当てられていることを確認する", async function () {
    const ownerBalance = await MusicToken.balanceOf(owner.address);
    expect(ownerBalance).to.equal(totalSupply * 10n ** 18n); // 18 decimals を考慮
  });

  it("トークン転送が正常に行われることを確認する", async function () {
    await MusicToken.transfer(addr1.address, 50n * 10n ** 18n);
    const addr1Balance = await MusicToken.balanceOf(addr1.address);
    expect(addr1Balance).to.equal(50n * 10n ** 18n);
  });

  it("残高不足によるトークン転送が失敗することを確認する", async function () {
    const initialOwnerBalance = await MusicToken.balanceOf(owner.address);

    await expect(
      MusicToken.connect(addr1).transfer(addr2.address, 1n * 10n ** 18n)
    ).to.be.revertedWith("Insufficient balance");

    expect(await MusicToken.balanceOf(owner.address)).to.equal(initialOwnerBalance);
  });

  it("トークンの許可と転送が正常に行われることを確認する", async function () {
    await MusicToken.approve(addr1.address, 100n * 10n ** 18n);
    expect(await MusicToken.allowance(owner.address, addr1.address)).to.equal(100n * 10n ** 18n);

    await MusicToken.connect(addr1).transferFrom(owner.address, addr2.address, 50n * 10n ** 18n);
    const addr2Balance = await MusicToken.balanceOf(addr2.address);
    expect(addr2Balance).to.equal(50n * 10n ** 18n);
  });
});